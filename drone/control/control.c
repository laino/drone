#include <stdio.h>
#include <unistd.h>
#include <sensors.c>
#include <math.h>
#include <time.h>

int main(){
  if (!bcm2835_init()){
    printf("Error initializing chipset\n");
    return 1;
  }
  
  if(sensors_open()){
    printf("Error opening sensors\n");
    return 1;
  }
  
  if(sensors_select_accel_range(SENSORS_ACCEL_RANGE_8G)){
    printf("Error selecting accel range\n");
    return 1;
  }
  
  if(sensors_select_gyro_range(SENSORS_GYRO_RANGE_2000)){
    printf("Error selecting gyro range\n");
    return 1;
  }

  float selftest_results[6];
  float temp = 0;
  int i;

  struct SENSORS_ACCEL_DATA accel = {0,0,0};
  struct SENSORS_GYRO_DATA gyro = {0,0,0};
  struct timespec tstart={0,0}, tend={0,0};
  
  if(sensors_mpu6050_selftest(selftest_results)){
    printf("Error executing selftest\n");
  }

  printf("SELFTEST: GYRO(%.3f %.3f %.3f) ACCEL(%.3f %.3f %.3f)\n",
      selftest_results[0],
      selftest_results[1],
      selftest_results[2],
      selftest_results[3],
      selftest_results[4],
      selftest_results[5]);
  
  for(i=0; i<6; i++){
    if(fabs(selftest_results[i]) > 0.14f){
      printf("Selftest failed!\n");
      return 1;
    }
  }

  printf("-------------------------------------------------\n");

  while(1){

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    sensors_read_all_data(&accel, SENSORS_ACCEL_RANGE_8G,
                          &gyro, SENSORS_GYRO_RANGE_2000,
                          &temp);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("ACCEL X: %7.2f Y: %7.2f Z: %7.2f TOTAL: %7.2f (g/s)\n",
            accel.x, 
            accel.y, 
            accel.z, 
            sqrt(accel.x*accel.x + accel.y*accel.y + accel.z*accel.z));

    printf("GYRO  X: %7.1f Y: %7.1f Z: %7.1f TOTAL: %7.1f (degrees/second)\n",
            gyro.x, 
            gyro.y, 
            gyro.z, 
            fabs(gyro.x) + fabs(gyro.y) + fabs(gyro.z));

    printf("TEMP   : %7.2f (celsius)      TIME READING: %7.5f (seconds)\n", 
          temp,
          ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
          ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    printf("\033[3F");

    usleep(100000);
  }
  
  sensors_close();
  bcm2835_close();
  return 0;
}
