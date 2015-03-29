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
 * Accelerometer registers of the MPU6050
 */
#define SENSORS_MPU6050_REGISTER_ACCEL_XHIGH 0x3B
#define SENSORS_MPU6050_REGISTER_ACCEL_XLOW  0x3C
#define SENSORS_MPU6050_REGISTER_ACCEL_YHIGH 0x3D
#define SENSORS_MPU6050_REGISTER_ACCEL_YLOW  0x3E
#define SENSORS_MPU6050_REGISTER_ACCEL_ZHIGH 0x3F
#define SENSORS_MPU6050_REGISTER_ACCEL_ZLOW  0x40

/*
 * Temperature registers of the MPU6050
 */

#define SENSORS_MPU6050_REGISTER_TEMP_HIGH 0x41
#define SENSORS_MPU6050_REGISTER_TEMP_LOW  0x42

/*
 * Gyro registers of the MPU6050
 */
#define SENSORS_MPU6050_REGISTER_GYRO_XHIGH 0x43
#define SENSORS_MPU6050_REGISTER_GYRO_XLOW  0x44
#define SENSORS_MPU6050_REGISTER_GYRO_YHIGH 0x45
#define SENSORS_MPU6050_REGISTER_GYRO_YLOW  0x46
#define SENSORS_MPU6050_REGISTER_GYRO_ZHIGH 0x47
#define SENSORS_MPU6050_REGISTER_GYRO_ZLOW  0x48

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
