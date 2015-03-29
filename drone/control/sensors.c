#include <bcm2835/bcm2835.h>
#include <unistd.h>
#include "sensors.h"

static int sensors_i2c_init_mpu6050() {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  // Select X gyro as clock source
  char data_clksource[] = {
    SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG,
    0b00000001
  };
      
  return bcm2835_i2c_write(data_clksource, 2);
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

static int sensors_read_register(char reg, char *result){
  return bcm2835_i2c_write(&reg, 1) ||
         bcm2835_i2c_read(result, 1);
}

int sensors_read_accel_data(struct SENSORS_ACCEL_DATA *out, int accel_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];
  int result, dividor;

  result = sensors_read_register(SENSORS_MPU6050_REGISTER_ACCEL_XHIGH, buffer) ||
           sensors_read_register(SENSORS_MPU6050_REGISTER_ACCEL_XLOW, buffer + 1) ||
           sensors_read_register(SENSORS_MPU6050_REGISTER_ACCEL_YHIGH, buffer + 2) ||
           sensors_read_register(SENSORS_MPU6050_REGISTER_ACCEL_YLOW, buffer + 3) ||
           sensors_read_register(SENSORS_MPU6050_REGISTER_ACCEL_ZHIGH, buffer + 4) ||
           sensors_read_register(SENSORS_MPU6050_REGISTER_ACCEL_ZLOW, buffer + 5);

  if(result) return 1;

  // dirty trick...
  dividor = 16384 >> accel_range;

  out->x = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / dividor;
  out->y = ((double) ((int16_t) (buffer[2] << 8) + buffer[3])) / dividor;
  out->z = ((double) ((int16_t) (buffer[4] << 8) + buffer[5])) / dividor;

  return 0;
}
