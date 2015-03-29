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

int sensors_read_accel_data(struct SENSORS_ACCEL_DATA *out, int accel_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];
  int ret, dividor;

  ret = sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_XHIGH, buffer)     ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_XLOW,  buffer + 1) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_YHIGH, buffer + 2) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_YLOW,  buffer + 3) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_ZHIGH, buffer + 4) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_ZLOW,  buffer + 5);

  if(ret) return 1;

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

int sensors_read_gyro_data(struct SENSORS_GYRO_DATA *out, int gyro_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];
  int ret;
  double dividor;

  ret = sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_XHIGH, buffer)     ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_XLOW,  buffer + 1) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_YHIGH, buffer + 2) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_YLOW,  buffer + 3) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_ZHIGH, buffer + 4) ||
        sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_ZLOW,  buffer + 5);

  if(ret) return 1;

  dividor = 131 / (double) (1 << gyro_range);

  out->x = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / dividor;
  out->y = ((double) ((int16_t) (buffer[2] << 8) + buffer[3])) / dividor;
  out->z = ((double) ((int16_t) (buffer[4] << 8) + buffer[5])) / dividor;

  return 0;
}
