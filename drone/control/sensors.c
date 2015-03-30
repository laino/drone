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
  
  // Disable all filters, leaves us with
  // 0ms accelerometer delay and 0.98ms gyro delay
  // See: Register 26 reference
  char data_config[] = {
    SENSORS_MPU6050_REGISTER_CONFIG,
    0b00000000
  };
      
  return bcm2835_i2c_write(data_clksource, 2) ||
         bcm2835_i2c_write(data_config, 2);
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

static inline int sensors_read_accel_data_raw(char buffer[6]){
  return bcm2835_i2c_write_read_rs(SENSORS_MPU6050_REGISTER_ACCEL_FIRST, 1, buffer, 6);
}

static inline int sensors_read_temp_data_raw(char buffer[2]){
  return bcm2835_i2c_write_read_rs(SENSORS_MPU6050_REGISTER_TEMP_FIRST, 1, buffer, 2);
}

static inline int sensors_read_gyro_data_raw(char buffer[6]){
  return bcm2835_i2c_write_read_rs(SENSORS_MPU6050_REGISTER_GYRO_FIRST, 1, buffer, 6);
}

static inline int sensors_read_all_data_raw(char buffer[14]){
  return bcm2835_i2c_write_read_rs(SENSORS_MPU6050_REGISTER_ACCEL_FIRST, 1, buffer, 14);
}

static void sensors_parse_accel_data(char buffer[6], struct SENSORS_ACCEL_DATA *out, int accel_range){
  int dividor = 16384 >> accel_range;

  out->x = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / dividor;
  out->y = ((double) ((int16_t) (buffer[2] << 8) + buffer[3])) / dividor;
  out->z = ((double) ((int16_t) (buffer[4] << 8) + buffer[5])) / dividor;
}

static void sensors_parse_temp_data(char buffer[2], double *out){
  *out = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / 340 + 36.53;
}

static void sensors_parse_gyro_data(char buffer[6], struct SENSORS_GYRO_DATA *out, int gyro_range){
  double dividor = 131 / (double) (1 << gyro_range);

  out->x = ((double) ((int16_t) (buffer[0] << 8) + buffer[1])) / dividor;
  out->y = ((double) ((int16_t) (buffer[2] << 8) + buffer[3])) / dividor;
  out->z = ((double) ((int16_t) (buffer[4] << 8) + buffer[5])) / dividor;
}

int sensors_read_accel_data(struct SENSORS_ACCEL_DATA *out, int accel_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];

  if(sensors_read_accel_data_raw(buffer)) return 1;

  sensors_parse_accel_data(buffer, out, accel_range);

  return 0;
}

int sensors_read_temp_data(double *degrees) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[2];

  if(sensors_read_temp_data_raw(buffer)) return 1;

  sensors_parse_temp_data(buffer, degrees);

  return 0;
}

int sensors_read_gyro_data(struct SENSORS_GYRO_DATA *out, int gyro_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];

  if(sensors_read_gyro_data_raw(buffer)) return 1;
  
  sensors_parse_gyro_data(buffer, out, gyro_range);
  
  return 0;
}

int sensors_read_all_data(struct SENSORS_ACCEL_DATA *accel, int accel_range,
                          struct SENSORS_GYRO_DATA *gyro, int gyro_range,
                          double *degrees) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[14];

  if(sensors_read_all_data_raw(buffer)) return 1;
  
  sensors_parse_accel_data(buffer, accel, accel_range);
  sensors_parse_temp_data(buffer + 6, degrees);
  sensors_parse_gyro_data(buffer + 8, gyro, gyro_range);

  return 0;
}

