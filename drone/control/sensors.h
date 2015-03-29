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
 * Power management (1) configuration register of the MPU6050
 *
 * | RESET | SLEEP | CYCLE | - | TEMP_DIS | CLKSEL0 | CLKSEL1 | CLKSEL2 |
 */
#define SENSORS_MPU6050_REGISTER_POWERMNGT1_CONFIG 0x6B

/*
 * Accelerometer registers of the MPU6050
 */
#define SENSORS_MPU6050_REGISTER_ACCEL_XHIGH 0x3B
#define SENSORS_MPU6050_REGISTER_ACCEL_XLOW  0x3C
#define SENSORS_MPU6050_REGISTER_ACCEL_YHIGH 0x3D
#define SENSORS_MPU6050_REGISTER_ACCEL_YLOW  0x3E
#define SENSORS_MPU6050_REGISTER_ACCEL_ZHIGH 0x3F
#define SENSORS_MPU6050_REGISTER_ACCEL_ZLOW  0x40

/*
 * MPU6050 Accelerometer Range. A bigger range allows
 * for measurement on a bigger scale at the cost of
 * precision.
 */
#define SENSORS_ACCEL_RANGE_2G 0
#define SENSORS_ACCEL_RANGE_4G 1
#define SENSORS_ACCEL_RANGE_8G 2
#define SENSORS_ACCEL_RANGE_16G 3

struct SENSORS_ACCEL_DATA {
  double x;
  double y;
  double z;
};
