#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
//#include <time.h>
//#include "i2cfunc.h"
#include <sys/mman.h>


#define PRU_ADDR        0x4A300000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU0_DRAM       0x00000         // Offset to DRAM
#define PRU1_DRAM       0x02000
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

//float    *pru0DRAM_32int_ptr;        // Points to the start of local DRAM
//float    *pru1DRAM_32int_ptr;        // Points to the start of local DRAM
volatile float *pru_sram; // Points to the start of the shared memory

#define I2CBUS 2
#define deviceADDR 0x68
#define SLEEP 0x6B
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



int i2c_open(unsigned char bus, unsigned char addr){
    int file;
    char filename[16];
    sprintf(filename,"/dev/i2c-%d", bus);
    if ((file = open(filename,O_RDWR)) < 0){
        fprintf(stderr, "i2c_open open error: %s\n", strerror(errno));
        return(file);
    }
    if (ioctl(file,I2C_SLAVE,addr) < 0){
        fprintf(stderr, "i2c_open ioctl error: %s\n", strerror(errno));
        return(-1);
    }
    return(file);
}

int i2c_close(int handle){
    if ((close(handle)) != 0){
        fprintf(stderr, "i2c_close error: %s\n", strerror(errno));
        return(-1);
    }
    return(0);
}

int i2c_read(int handle, unsigned char* buf, unsigned int length){
    if (read(handle, buf, length) != length){
        fprintf(stderr, "i2c_read error: %s\n", strerror(errno));
        return(-1);
    }
  return(length);
}




int i2c_write(int handle, unsigned char* buf, unsigned int length)
{
  if (write(handle, buf, length) != length){
    fprintf(stderr, "i2c_write error: %s\n", strerror(errno));
    return(-1);
  }
  return(length);
}






int main(void){
  
    float    *pru;
    int fd;
    fd = open ("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        printf ("ERROR: could not open /dev/mem.\n\n");
        return 1;
    }
    pru = mmap (0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    
    printf("\nPRU-pointer points to: %p \n", pru);
    if (pru == MAP_FAILED) {
        printf ("ERROR: could not map memory.\n\n");
        return 1;
    }
    printf ("Using /dev/mem.\n");
    
    pru_sram = pru + PRU_SHAREDMEM/4;
    printf("\nPRU-SHAREDMEM points to: %p \n", pru_sram);
    
    int handle=i2c_open(I2CBUS, deviceADDR);
    
    unsigned char w_AXH[1] = {ACC_X_H};
    unsigned char w_AXL[1] = {ACC_X_L};
    unsigned char w_AYH[1] = {ACC_Y_H};
    unsigned char w_AYL[1] = {ACC_Y_L};
    unsigned char w_AZH[1] = {ACC_Z_H};
    unsigned char w_AZL[1] = {ACC_Z_L};
    unsigned char r_AXH[1], r_AXL[1], r_AYH[1], r_AYL[1], r_AZH[1], r_AZL[1];
    
    unsigned char w_GXH[1] = {GYRO_X_H};
    unsigned char w_GXL[1] = {GYRO_X_L};
    unsigned char w_GYH[1] = {GYRO_Y_H};
    unsigned char w_GYL[1] = {GYRO_Y_L};
    unsigned char w_GZH[1] = {GYRO_Z_H};
    unsigned char w_GZL[1] = {GYRO_Z_L};
    unsigned char r_GXH[1], r_GXL[1], r_GYH[1], r_GYL[1], r_GZH[1], r_GZL[1];
    
    
    while(1){
    i2c_write(handle,w_AXH,1);
    i2c_read(handle,r_AXH,1);
    i2c_write(handle,w_AXL,1);
    i2c_read(handle,r_AXL,1);
   
    i2c_write(handle,w_AYH,1);
    i2c_read(handle,r_AYH,1);
    i2c_write(handle,w_AYL,1);
    i2c_read(handle,r_AYL,1);

    i2c_write(handle,w_AZH,1);
    i2c_read(handle,r_AZH,1);
    i2c_write(handle,w_AZL,1);
    i2c_read(handle,r_AZL,1);
    
    i2c_write(handle,w_GXH,1);
    i2c_read(handle,r_GXH,1);
    i2c_write(handle,w_GXL,1);
    i2c_read(handle,r_GXL,1);
   
    i2c_write(handle,w_GYH,1);
    i2c_read(handle,r_GYH,1);
    i2c_write(handle,w_GYL,1);
    i2c_read(handle,r_GYL,1);

    i2c_write(handle,w_GZH,1);
    i2c_read(handle,r_GZH,1);
    i2c_write(handle,w_GZL,1);
    i2c_read(handle,r_GZL,1);
   
    
    //PUT TOGETHER THE 2-COMPLEMENT SIGNED INTEGER
    int16_t ACC_X = (r_AXH[0]<<8)+r_AXL[0];
    int16_t ACC_Y = (r_AYH[0]<<8)+r_AYL[0];
    int16_t ACC_Z = (r_AZH[0]<<8)+r_AZL[0];
    
    int16_t GYRO_X = (r_GXH[0]<<8)+r_GXL[0];
    int16_t GYRO_Y = (r_GYH[0]<<8)+r_GYL[0];
    int16_t GYRO_Z = (r_GZH[0]<<8)+r_GZL[0];
    

    
    //CONVERT THE INTEGERS TO MEANINGFUL QUANTITIES & WRITE TO THE PRU's SHARED RAM
    pru_sram[0] = ((float) ACC_X)/16384 -0.0222;
    pru_sram[1] = ((float) -ACC_Y)/16384 -0.0048;                                           // (MY) SENSOR IS UPSIDE DOWN. ROTATION MATRIX: 180 DEGREES ABOUT X-AXIS
    pru_sram[2] = ((float) -ACC_Z)/16384 -0.1791;   // Adjusting for bias
    pru_sram[3] = ((float) GYRO_X)/131 +2.4399;
    pru_sram[4] = ((float) -GYRO_Y)/131 -0.1591;
    pru_sram[5] = ((float) -GYRO_Z)/131 -0.2010;
    
    printf("M1: %d \t M2: %d \t M3: %d \t M4: %d \t Base Thrust: %f \n", (int) (pru_sram[14]+pru_sram[10]), (int) (pru_sram[14]+pru_sram[11]), (int) (pru_sram[14]+pru_sram[12]), (int) (pru_sram[14]+pru_sram[13]), pru_sram[14]);
    
    //printf("ACC: %f \t %f \t %f \t\t\t\t\t\t GYRO: %f \t %f \t %f \n", pru_sram[0], pru_sram[1], pru_sram[2], pru_sram[3], pru_sram[4],pru_sram[5]);
    //printf("ROLL: %f \t %f\n", pru_sram[14]*180/3.1415, pru_sram[15]*180/3.1415);    
    //printf("MOTOR 1: %f \t MOTOR 2: %f \t MOTOR 3: %f \t MOTOR 4: %f \n", pru_sram[10], pru_sram[11], pru_sram[12], pru_sram[13]);
        
    }
    
    //printf("ACC: %f \t %f \t %f \t\t\t\t\t\t GYRO: %f \t %f \t %f \n", pru_sram[0], pru_sram[1], pru_sram[2], pru_sram[3], pru_sram[4],pru_sram[5]);
    i2c_close(handle);
    
    
}








    //CONVERT THE INTEGERS TO MEANINGFUL QUANTITIES
    /*
    float cACC_X = ((float) ACC_X)/16384;
    float cACC_Y = ((float) -ACC_Y)/16384; // MY SENSOR IS UPSIDE DOWN. ROTATION MATRIX: 180 DEGREES ABOUT X-AXIS
    float cACC_Z = ((float) -ACC_Z)/16384;
    
    float cGYRO_X = ((float) GYRO_X)/131;
    float cGYRO_Y = ((float) -GYRO_Y)/131;
    float cGYRO_Z = ((float) -GYRO_Z)/131;
    */
    //printf("ACC: %f \t %f \t %f \t\t\t\t\t\t GYRO: %f \t %f \t %f \n", cACC_X, cACC_Y, cACC_Z, cGYRO_X, cGYRO_Y, cGYRO_Z);
    