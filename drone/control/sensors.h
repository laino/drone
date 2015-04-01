/*
 * I2C bus address of the MPU6050
 */
#define SENSORS_MPU6050_ADDRESS 0x68

/*
 * Accelerometer configuration register of the MPU6050
 * 
 * | XST | YST | ZST | AFS0 | AFS1 | - | - | - |
 *
 * *ST = Self test
 * AFS* = Accelerometer range selection (2bit)
 */
#define SENSORS_MPU6050_REGISTER_ACCEL_CONFIG 0x1C

/*
 * Gyroscope configuration register of the MPU6050
 * 
 * | XG_ST | YG_ST | ZG_ST | FS_SEL0 | FS_SEL1 | - | - | - |
 *
 * *ST = Self test
 * FS_SEL* = Gyroscope range selection (2bit)
 */
#define SENSORS_MPU6050_REGISTER_GYRO_CONFIG 0x18

/*
 * Power management (1) configuration register of the MPU6050
 *
 * | RESET | SLEEP | CYCLE | - | TEMP_DIS | CLKSEL0 | CLKSEL1 | CLKSEL2 |
 */
#define SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG 0x1A

/*
 * Signal path reset register of the MPU6050
 *
 * | - | - | - | - | - | GYRO_RESET | ACCEL_RESET | TEMP_RESET |
 */
#define SENSORS_MPU6050_REGISTER_SPR 0x68

/*
 * Configuration register of the MPU6050
 *
 * | - | - | FSYNC0 | FSYNC1 | FSYNC2 | DLPF0 | DLPF1 | DLPF2 |
 */
#define SENSORS_MPU6050_REGISTER_CONFIG 0x6B

/*
 * First, of the four, selftest register of the MPU6050.
 * Refer to the register spec for their contents.
 */
#define SENSORS_MPU6050_REGISTER_SELFTEST_FIRST 0x0D

/*
 * First accelerometer register of the MPU6050. For the
 * following register increment by one (1) each.
 *
 * The registers are in the following order (6 registers):
 *
 * XHIGH, XLOW, YHIGH, YLOW, ZHIGH, ZLOW
 */
#define SENSORS_MPU6050_REGISTER_ACCEL_FIRST 0x3B

/*
 * First temperature register of the MPU6050. For the
 * following register increment by one (1) each.
 *
 * The registers are in the following order (2 registers):
 *
 * HIGH, LOW
 */
#define SENSORS_MPU6050_REGISTER_TEMP_FIRST 0x41

/*
 * First gyro register of the MPU6050. For the
 * following register increment by one (1) each.
 *
 * The registers are in the following order (6 registers):
 *
 * XHIGH, XLOW, YHIGH, YLOW, ZHIGH, ZLOW
 */
#define SENSORS_MPU6050_REGISTER_GYRO_FIRST 0x43

/*
 * Fifo configuration register of the MPU6050.
 *
 * 
 * | TEMP | Gyro_X | Gyro_Y | Gyro_Z | Accel | SLV_2 | SLV_1 | SLV_0 |
 *
 */
#define SENSORS_MPU6050_REGISTER_FIFO_ENABLE 0x23

/*
 * Fifo configuration.
 *
 * If more than one kind of value is added to the FIFO
 * queue, values will have the following order:
 *
 * Acceleration, Temperature, Gyroscope, ...
 *
 * And must be consumed in the same.
 */
#define SENSORS_FIFO_ACCEL 0x08
#define SENSORS_FIFO_TEMP  0x80
#define SENSORS_FIFO_GYRO  0x70

/*
 * MPU6050 Accelerometer Range. A bigger range allows
 * for measurement on a bigger scale at the cost of
 * precision.
 */
#define SENSORS_ACCEL_RANGE_2G 0
#define SENSORS_ACCEL_RANGE_4G 1
#define SENSORS_ACCEL_RANGE_8G 2
#define SENSORS_ACCEL_RANGE_16G 3

/*
 * Accelerometer output. Expressed as g-force experienced
 * by the sensor along the specified axis.
 */
struct SENSORS_ACCEL_DATA {
  float x;
  float y;
  float z;
};

/*
 * MPU6050 Gyroscope range. Expressed in degrees/second. 
 */

#define SENSORS_GYRO_RANGE_250 0
#define SENSORS_GYRO_RANGE_500 1
#define SENSORS_GYRO_RANGE_1000 2
#define SENSORS_GYRO_RANGE_2000 3

/*
 * Gyroscope output expressed as degrees/second around the
 * specified axis.
 */
struct SENSORS_GYRO_DATA {
  float x;
  float y;
  float z;
};

/*
 * Holds gyroscope and accelerometer
 * calibration data.
 */
struct SENSORS_CALIBRATION_DATA {
  int16_t accel_bias_x;
  int16_t accel_bias_y;
  int16_t accel_bias_z;
  int16_t gyro_bias_x;
  int16_t gyro_bias_y;
  int16_t gyro_bias_z;
};
