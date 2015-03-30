#include <bcm2835/bcm2835.h>
#include <unistd.h>
#include "sensors.h"

static int sensors_i2c_init_mpu6050() {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  // Select Z gyro as clock source
  char data_clksource[] = {
    SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG,
    0b00000011
  };
      
  return bcm2835_i2c_write(data_clksource, 2);
}

static int sensors_i2c_read_register(char reg, char *result){
  return bcm2835_i2c_write(&reg, 1) ||
         bcm2835_i2c_read(result, 1);
}

int sensors_open(){
  bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_148);
  bcm2835_i2c_begin();
  return sensors_i2c_init_mpu6050();
}

void sensors_close(){
  bcm2835_i2c_end();
}

int sensors_select_accel_range(int accel_range){
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char data[] = { SENSORS_MPU6050_REGISTER_ACCEL_CONFIG, 
                  accel_range << 3 };

  return bcm2835_i2c_write(data, 2);
}

int sensors_select_gyro_range(int gyro_range){
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char data[] = { SENSORS_MPU6050_REGISTER_GYRO_CONFIG, 
                  gyro_range << 3 };

  return bcm2835_i2c_write(data, 2);
}

static inline int sensors_read_data6(char buffer[6], char a, char b, char c, char d, char e, char f){
  return sensors_i2c_read_register(a, buffer)     ||
         sensors_i2c_read_register(b, buffer + 1) ||
         sensors_i2c_read_register(c, buffer + 2) ||
         sensors_i2c_read_register(d, buffer + 3) ||
         sensors_i2c_read_register(e, buffer + 4) ||
         sensors_i2c_read_register(f, buffer + 5);
}

static int sensors_read_accel_data_raw(char buffer[6]){
  return sensors_read_data6(buffer,
         SENSORS_MPU6050_REGISTER_ACCEL_XHIGH,
         SENSORS_MPU6050_REGISTER_ACCEL_XLOW,
         SENSORS_MPU6050_REGISTER_ACCEL_YHIGH,
         SENSORS_MPU6050_REGISTER_ACCEL_YLOW,
         SENSORS_MPU6050_REGISTER_ACCEL_ZHIGH,
         SENSORS_MPU6050_REGISTER_ACCEL_ZLOW);
}

int sensors_read_accel_data(struct SENSORS_ACCEL_DATA *out, int accel_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];
  int dividor;

  if(sensors_read_accel_data_raw(buffer)) return 1;

  dividor = 16384 >> accel_range;

  out->x = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / dividor;
  out->y = ((double) ((int16_t) (buffer[2] << 8) + buffer[3])) / dividor;
  out->z = ((double) ((int16_t) (buffer[4] << 8) + buffer[5])) / dividor;

  return 0;
}

int sensors_read_temp_data(double *degrees) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];
  int ret;

  ret = sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_TEMP_HIGH, buffer) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_TEMP_LOW,  buffer + 1);

  if(ret) return 1;

  *degrees = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / 340 + 36.53;

  return 0;
}

static int sensors_read_gyro_data_raw(char buffer[6]){
  return sensors_read_data6(buffer,
         SENSORS_MPU6050_REGISTER_GYRO_XHIGH,
         SENSORS_MPU6050_REGISTER_GYRO_XLOW,
         SENSORS_MPU6050_REGISTER_GYRO_YHIGH,
         SENSORS_MPU6050_REGISTER_GYRO_YLOW,
         SENSORS_MPU6050_REGISTER_GYRO_ZHIGH,
         SENSORS_MPU6050_REGISTER_GYRO_ZLOW);
}

int sensors_read_gyro_data(struct SENSORS_GYRO_DATA *out, int gyro_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];
  double dividor;

  if(sensors_read_gyro_data_raw(buffer)) return 1;

  dividor = 131 / (double) (1 << gyro_range);

  out->x = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / dividor;
  out->y = ((double) ((int16_t) (buffer[2] << 8) + buffer[3])) / dividor;
  out->z = ((double) ((int16_t) (buffer[4] << 8) + buffer[5])) / dividor;

  return 0;
}
