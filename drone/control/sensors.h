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
#define SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG 0x6B

/*
 * First accelerometer register of the MPU6050. For the
 * following register increment by one (1) each.
 *
 * The registers are in the following order (6 registers):
 *
 * XHIGH, XLOW, YHIGH, YLOW, ZHIGH, ZLOW
 */
#define SENSORS_MPU6050_REGISTER_ACCEL_FIRST "\x3B"

/*
 * First temperature register of the MPU6050. For the
 * following register increment by one (1) each.
 *
 * The registers are in the following order (2 registers):
 *
 * HIGH, LOW
 */
#define SENSORS_MPU6050_REGISTER_TEMP_FIRST "\x41"

/*
 * First gyro register of the MPU6050. For the
 * following register increment by one (1) each.
 *
 * The registers are in the following order (6 registers):
 *
 * XHIGH, XLOW, YHIGH, YLOW, ZHIGH, ZLOW
 */
#define SENSORS_MPU6050_REGISTER_GYRO_FIRST "\x43"

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
  double x;
  double y;
  double z;
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
  double x;
  double y;
  double z;
};
