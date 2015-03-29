#include <stdio.h>
#include <unistd.h>
#include <sensors.c>
#include <math.h>

int main(){
  if (!bcm2835_init()){
    return 1;
  }
  sensors_open();
  
  if(sensors_select_accel_range(SENSORS_ACCEL_RANGE_16G)){
    printf("Error selecting accel range");
    return 1;
  }

  struct SENSORS_ACCEL_DATA data;

  while(1){
    sensors_read_accel_data(&data, SENSORS_ACCEL_RANGE_16G);
    printf("X: %6.3f Y: %6.3f Z: %6.3f TOTAL: %6.3f\n",
            data.x, 
            data.y, 
            data.z, 
            sqrt(data.x*data.x + data.y*data.y + data.z*data.z));
    usleep(100000);
  }
  
  sensors_close();
  bcm2835_close();
  return 0;
}
