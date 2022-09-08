#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <errno.h>
#include <fcntl.h>

//COMPILE BY: gcc smbusI2C.c -o smbc -li2c
#define ACC_X_H 0x3B
#define ACC_X_L 0x3C
#define ACC_Y_H 0x3D
#define ACC_Y_L 0x3E
#define ACC_Z_H 0x3F
#define ACC_Z_L 0x40
#define GYRO_X_H 0x43
#define GYRO_X_L 0x44
#define GYRO_Y_H 0x45
#define GYRO_Y_L 0x46
#define GYRO_Z_H 0x47
#define GYRO_Z_L 0x48
#define WHOAMI 0x75

int main(){
    
    int file;
    int adapter_nr = 2; /* probably dynamically determined */
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    file = open(filename, O_RDWR);
    if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    
    
    int addr = 0x68; /* The I2C address */

    if (ioctl(file, I2C_SLAVE, addr) < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }
  
  for(int i=0;i<10000;i++){
  int16_t ACCXH = i2c_smbus_read_byte_data(file,ACC_X_H);
  int16_t ACCXL = i2c_smbus_read_byte_data(file,ACC_X_L);
  int16_t ACCYH = i2c_smbus_read_byte_data(file,ACC_Y_H);
  int16_t ACCYL = i2c_smbus_read_byte_data(file,ACC_Y_L);
  int16_t ACCZH = i2c_smbus_read_byte_data(file,ACC_Z_H);
  int16_t ACCZL = i2c_smbus_read_byte_data(file,ACC_Z_L); 
  
  int16_t GYROXH = i2c_smbus_read_byte_data(file,GYRO_X_H);
  int16_t GYROXL = i2c_smbus_read_byte_data(file,GYRO_X_L);
  int16_t GYROYH = i2c_smbus_read_byte_data(file,GYRO_Y_H);
  int16_t GYROYL = i2c_smbus_read_byte_data(file,GYRO_Y_L);
  int16_t GYROZH = i2c_smbus_read_byte_data(file,GYRO_Z_H);
  int16_t GYROZL = i2c_smbus_read_byte_data(file,GYRO_Z_L);
  
  int16_t ACCX = (ACCXH<<8) + ACCXL;
  int16_t ACCY = (ACCYH<<8) + ACCYL;
  int16_t ACCZ = (ACCZH<<8) + ACCZL;
  
  int16_t GYROX = (GYROXH<<8) + GYROXL;
  int16_t GYROY = (GYROYH<<8) + GYROYL;
  int16_t GYROZ = (GYROZH<<8) + GYROZL;
  }
  //printf("ACCX: %d,%d \t ACCY %d,%d\n", ACCXH, ACCXL, ACCYH, ACCYL);
  //printf("ACCX: %d \t ACCY: %d\n", ACCX, ACCY); 
  //printf("ACC: %d \t %d \t %d \t\t\t\t\t\t GYRO: %d \t %d \t %d \n", ACCX, ACCY, ACCZ, GYROX, GYROY, GYROZ);

  
  
  
}