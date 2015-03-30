#include <stdio.h>
#include <unistd.h>
#include <sensors.c>
#include <math.h>
#include <time.h>

int main(){
  if (!bcm2835_init()){
    return 1;
  }
  sensors_open();
  
  if(sensors_select_accel_range(SENSORS_ACCEL_RANGE_8G)){
    printf("Error selecting accel range");
    return 1;
  }
  
  if(sensors_select_gyro_range(SENSORS_GYRO_RANGE_250)){
    printf("Error selecting gyro range");
    return 1;
  }

  struct SENSORS_ACCEL_DATA accel;
  struct SENSORS_GYRO_DATA gyro;
  struct timespec tstart={0,0}, tend={0,0};

  double temp;

  printf("\033[2J");

  while(1){

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    sensors_read_accel_data(&accel, SENSORS_ACCEL_RANGE_8G);
    sensors_read_gyro_data(&gyro, SENSORS_GYRO_RANGE_250);
    sensors_read_temp_data(&temp);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("\033[0;0H");
    printf("-----------------------------------------------------\n");
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

    usleep(100000);
  }
  
  sensors_close();
  bcm2835_close();
  return 0;
}
