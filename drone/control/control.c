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
  
  if(sensors_select_accel_range(SENSORS_ACCEL_RANGE_16G)){
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

  while(1){

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    sensors_read_accel_data(&accel, SENSORS_ACCEL_RANGE_16G);
    sensors_read_gyro_data(&gyro, SENSORS_GYRO_RANGE_250);
    sensors_read_temp_data(&temp);

    clock_gettime(CLOCK_MONOTONIC, &tend);


    printf("-----------------------------------------------------\n");
    printf("ACCEL X: %6.3f Y: %6.3f Z: %6.3f TOTAL: %6.3f (g/s) \n",
            accel.x, 
            accel.y, 
            accel.z, 
            sqrt(accel.x*accel.x + accel.y*accel.y + accel.z*accel.z));

    printf("GYRO  X: %6.3f Y: %6.3f Z: %6.3f TOTAL: %6.3f (degrees/second)\n",
            gyro.x, 
            gyro.y, 
            gyro.z, 
            gyro.x + gyro.y + gyro.z);

    printf("TEMP   : %6.3f (C) TIME READING: %.5f (seconds)\n", 
          temp,
          ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
          ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    sleep(1);
  }
  
  sensors_close();
  bcm2835_close();
  return 0;
}
