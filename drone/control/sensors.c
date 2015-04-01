#include <bcm2835/bcm2835.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "sensors.h"

/*
 * Read a single register of an i2c device.
 */
static inline int sensors_i2c_read_register(char reg, char *result){
  return bcm2835_i2c_write(&reg, 1) ||
         bcm2835_i2c_read(result, 1);
}

static inline int sensors_i2c_read_register_multi(char reg, char buffer[], int len){
  return bcm2835_i2c_write_read_rs(&reg, 1, buffer, len);
}

/*
 * Initialize the MPU6050 chip by disabling powersave,
 * selecting the Z gyro as the clock source and disabling
 * the digital low pass filter (DLPF).
 */
static int sensors_i2c_init_mpu6050() {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  // Reset device
  char data_reset[] = {
    SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG,
    0x80
  };
  
  // Reset device signal paths
  char data_spr[] = {
    SENSORS_MPU6050_REGISTER_SPR,
    0x07
  };
  
  // Select Z gyro as clock source
  char data_powermngt[] = {
    SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG,
    0x03
  };
  
  // Disable all filters, leaves us with
  // 0ms accelerometer delay and 0.98ms gyro delay
  // See: Register 26 reference
  char data_config[] = {
    SENSORS_MPU6050_REGISTER_CONFIG,
    0x00
  };
  
  // Begin device reset
  if(bcm2835_i2c_write(data_reset, 2)) return 1;
  
  // Wait until the reset is done
  // Register map spec says 100ms (see 4.18)
  usleep(100000);

  // Begin SPR reset
  if(bcm2835_i2c_write(data_spr, 2)) return 1;
  
  // Wait until the SPR reset is done
  // Register map spec says 100ms (see 4.18)
  usleep(100000);

  if(bcm2835_i2c_write(data_powermngt, 2) ||
     bcm2835_i2c_write(data_config, 2)) return 1;

  // Without this wait the device will deliver
  // wrong values after a cold start for some reason
  usleep(100000);

  return 0;
}

/*
 * Initialize the I2C bus and connected devices.
 */
int sensors_open(){
  bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_148);
  bcm2835_i2c_begin();
  if(sensors_i2c_init_mpu6050()) return 1;
  return 0;
}

/*
 * Free the I2C bus.
 */
void sensors_close(){
  bcm2835_i2c_end();
}

/*
 * Select the measurement range for the accelerometer
 * of the MPU6050 chip.
 */
int sensors_select_accel_range(int accel_range){
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char data[] = { SENSORS_MPU6050_REGISTER_ACCEL_CONFIG, 
                  accel_range << 3 };

  return bcm2835_i2c_write(data, 2);
}

/*
 * Select the measurement range for the gyroscope 
 * of the MPU6050 chip.
 */
int sensors_select_gyro_range(int gyro_range){
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char data[] = { SENSORS_MPU6050_REGISTER_GYRO_CONFIG, 
                  gyro_range << 3 };

  return bcm2835_i2c_write(data, 2);
}

static inline int sensors_read_accel_data_raw(char buffer[6]){
  return sensors_i2c_read_register_multi(SENSORS_MPU6050_REGISTER_ACCEL_FIRST, buffer, 6);
}

static inline int sensors_read_temp_data_raw(char buffer[2]){
  return sensors_i2c_read_register_multi(SENSORS_MPU6050_REGISTER_TEMP_FIRST, buffer, 2);
}

static inline int sensors_read_gyro_data_raw(char buffer[6]){
  return sensors_i2c_read_register_multi(SENSORS_MPU6050_REGISTER_GYRO_FIRST, buffer, 6);
}

static inline int sensors_read_all_data_raw(char buffer[14]){
  return sensors_i2c_read_register_multi(SENSORS_MPU6050_REGISTER_ACCEL_FIRST, buffer, 14);
}

static void sensors_parse_accel_data(char buffer[6], struct SENSORS_ACCEL_DATA *out, int accel_range){
  float dividor = 16384 >> accel_range;

  out->x = ((float) ((int16_t) (buffer[0] << 8) | buffer[1])) / dividor;
  out->y = ((float) ((int16_t) (buffer[2] << 8) | buffer[3])) / dividor;
  out->z = ((float) ((int16_t) (buffer[4] << 8) | buffer[5])) / dividor;
}

static void sensors_parse_temp_data(char buffer[2], float *out){
  *out = ((float) ((int16_t) (buffer[0] << 8) + buffer[1])) / 340.f + 36.53f;
}

static void sensors_parse_gyro_data(char buffer[6], struct SENSORS_GYRO_DATA *out, int gyro_range){
  float dividor = 131 / (1 << gyro_range);

  out->x = ((float) ((int16_t) (buffer[0] << 8) | buffer[1])) / dividor;
  out->y = ((float) ((int16_t) (buffer[2] << 8) | buffer[3])) / dividor;
  out->z = ((float) ((int16_t) (buffer[4] << 8) | buffer[5])) / dividor;
}

/*
 * Read the current accelerometer data from the MPU6050 chip.
 */
int sensors_read_accel_data(struct SENSORS_ACCEL_DATA *out, int accel_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];

  if(sensors_read_accel_data_raw(buffer)) return 1;

  sensors_parse_accel_data(buffer, out, accel_range);

  return 0;
}

/*
 * Read the current temperature data from the MPU6050 chip.
 */
int sensors_read_temp_data(float *degrees) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[2];

  if(sensors_read_temp_data_raw(buffer)) return 1;

  sensors_parse_temp_data(buffer, degrees);

  return 0;
}

/*
 * Read the current gyroscope data from the MPU6050 chip.
 */
int sensors_read_gyro_data(struct SENSORS_GYRO_DATA *out, int gyro_range) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[6];

  if(sensors_read_gyro_data_raw(buffer)) return 1;
  
  sensors_parse_gyro_data(buffer, out, gyro_range);
  
  return 0;
}

/*
 * Read all current sensor data from the MPU6050 chip.
 */
int sensors_read_all_data(struct SENSORS_ACCEL_DATA *accel, int accel_range,
                          struct SENSORS_GYRO_DATA *gyro, int gyro_range,
                          float *degrees) {
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);

  char buffer[14];

  if(sensors_read_all_data_raw(buffer)) return 1;
  
  sensors_parse_accel_data(buffer, accel, accel_range);
  sensors_parse_temp_data(buffer + 6, degrees);
  sensors_parse_gyro_data(buffer + 8, gyro, gyro_range);

  return 0;
}

/*
 * Attempts to calibrate the gyro and acceleration
 * sensors for the given duration. It will
 * improve upon the given calibration data if it is
 * not empty.
 */
int sensors_mpu6050_calibrate(struct SENSORS_CALIBRATION_DATA *data, int duration){

}

/* 
 * Performs a selftest on the MPU6050.
 * The results are saved in "out", as
 * floating-comma numbers in the following order:
 * GyroX, GyroY, GyroZ, AccelX, AccelY, AccelZ
 *
 * No number should be greater than 0.14 (14%).
 * If a value is greater than 14%, the sensor failed
 * the self-test.
 */
int sensors_mpu6050_selftest(float out[]){
  bcm2835_i2c_setSlaveAddress(SENSORS_MPU6050_ADDRESS);
  
  char buffer[4], results[6];
  float factory_trim[6];
  int i;

  char data_gyro_selftest[] = {
    SENSORS_MPU6050_REGISTER_GYRO_CONFIG,
    0xE0 | (SENSORS_GYRO_RANGE_250 << 3)
  };
  
  char data_accel_selftest[] = {
    SENSORS_MPU6050_REGISTER_ACCEL_CONFIG,
    0xE0 | (SENSORS_ACCEL_RANGE_8G << 3)
  };
  
  char data_gyro_saved[] = {SENSORS_MPU6050_REGISTER_GYRO_CONFIG, 0};
  char data_accel_saved[] = {SENSORS_MPU6050_REGISTER_ACCEL_CONFIG, 0};

  // Save previous settings
  sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_GYRO_CONFIG, data_gyro_saved + 1);
  sensors_i2c_read_register(SENSORS_MPU6050_REGISTER_ACCEL_CONFIG, data_accel_saved + 1);
  
  // Start gyro & accel selftest
  if(bcm2835_i2c_write(data_gyro_selftest, 2) ||
     bcm2835_i2c_write(data_accel_selftest, 2)) return 1;
  
  usleep(100000);
  
  // Read selftest data
  if(sensors_i2c_read_register_multi(
        SENSORS_MPU6050_REGISTER_SELFTEST_FIRST,
        buffer, 4)) return 1;
  

  // Stop gyro & accel selftest
  if(bcm2835_i2c_write(data_gyro_saved, 2) ||
     bcm2835_i2c_write(data_accel_saved, 2)) return 1;

  usleep(100000);
  
  // gyroscope results:
  results[0] =   buffer[0] & 0x1F;
  results[1] =   buffer[1] & 0x1F;
  results[2] =   buffer[2] & 0x1F;
  
  // accelerometer results:
  results[3] = ((buffer[0] & 0xE0) >> 3) | ((buffer[3] & 0x30) >> 4);
  results[4] = ((buffer[1] & 0xE0) >> 3) | ((buffer[3] & 0x0C) >> 2);
  results[5] = ((buffer[2] & 0xE0) >> 3) | ( buffer[3] & 0x03);
  
  factory_trim[0] =  25.f * 131.f * pow(1.046f, (float) (results[0] - 1));
  factory_trim[1] = -25.f * 131.f * pow(1.046f, (float) (results[1] - 1));
  factory_trim[2] =  25.f * 131.f * pow(1.046f, (float) (results[2] - 1));
  
  factory_trim[3] = 4096.f * 0.34f * pow(0.92f / 0.34f, ((float) (results[3] - 1)) / 30.0f);
  factory_trim[4] = 4096.f * 0.34f * pow(0.92f / 0.34f, ((float) (results[4] - 1)) / 30.0f);
  factory_trim[5] = 4096.f * 0.34f * pow(0.92f / 0.34f, ((float) (results[5] - 1)) / 30.0f);

  for(i=0; i<6; i++){
    out[i] = (results[i] - factory_trim[i]) / factory_trim[i] + 1.f; 
  }

  return 0;
}

