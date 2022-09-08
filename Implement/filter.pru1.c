#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"
#include <pru_ctrl.h>
#include <math.h>

#define PRU1_DRAM       0x00000         // FROM PRU1's PERSPECTICE. Absolute addr == 0x02000
#define PRU_SHAREDMEM   0x10000          // Offset to shared memory from PRU 1, Absolute addr == 0x10000

volatile register uint32_t __R30;
volatile register uint32_t __R31;

volatile float *pru_sram = (float*) (PRU_SHAREDMEM); //[ACCX[0], ACCY[1], ACCZ[2], GYROX[3], GYROY[4], GYROZ[5], ... , Motor1[10], Motor2[11], Motor3[12], Motor4[13]

const float h = 0.000025; //Approx 400 Hz I2C communication, or should this depend on this functions speed?
const float cmpf_gamma = 0.95;
const float pi = 3.141592653589793;
const float bias_ACCX;
const float bias_ACCY;
const float bias_ACCZ;
const float bias_GYROX;
const float bias_GYROY;
const float bias_GYROZ;


const float KLQR[4][5] = {{-3.6219, -0.8290, -3.5319, -0.8075, -0.4907},
                         {-3.6219, -0.8290, 3.5319, 0.8075, 0.4907},
                         {3.6219, 0.8290, 3.5319, 0.8075, -0.4907},
                         {3.6219, 0.8290, -3.5319, -0.8075, 0.4907} };


volatile float roll_est;
volatile float pitch_est;
volatile float yaw_est;
volatile float yawRate_est=0;

void compFilter(){   //IMPLEMENT MUTEX
	
	
    float accRollEst = atan2f(pru_sram[1],pru_sram[2]);  
	float accPitchEst = atan2f(-pru_sram[0], (float) fabs(pru_sram[1]+pru_sram[2])); 	
	float gyroRollEst = h*pru_sram[3]*pi/180;                                             
	float gyroPitchEst = h*pru_sram[4]*pi/180;
	float gyroYawEst =  h*pru_sram[5]*pi/180; 

	roll_est = (1-cmpf_gamma)*accRollEst + cmpf_gamma*(gyroRollEst+roll_est);
	pitch_est = (1-cmpf_gamma)*accPitchEst + cmpf_gamma*(gyroPitchEst+pitch_est);
	yawRate_est = cmpf_gamma*(gyroYawEst+yaw_est);

}


void LQR(){     //IMPLEMENT MUTEX
    pru_sram[10] = 200*(KLQR[0][0]*roll_est + KLQR[0][1]*pru_sram[3] + KLQR[0][2]*pitch_est + KLQR[0][3]*pru_sram[4] + KLQR[0][4]*pru_sram[5]); 
    pru_sram[11] = 200*(KLQR[1][0]*roll_est + KLQR[1][1]*pru_sram[3] + KLQR[1][2]*pitch_est + KLQR[1][3]*pru_sram[4] + KLQR[1][4]*pru_sram[5]);
    pru_sram[12] = 200*(KLQR[2][0]*roll_est + KLQR[2][1]*pru_sram[3] + KLQR[2][2]*pitch_est + KLQR[2][3]*pru_sram[4] + KLQR[2][4]*pru_sram[5]);
    pru_sram[13] = 200*(KLQR[3][0]*roll_est + KLQR[3][1]*pru_sram[3] + KLQR[3][2]*pitch_est + KLQR[3][3]*pru_sram[4] + KLQR[3][4]*pru_sram[5]);
}



void main(void){
    //__halt();
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0; /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
    
    /*
    uint32_t cycle, stall;
    PRU1_CTRL.CTRL_bit.CTR_EN = 1;
    compFilter();
    LQR();
    __delay_cycles(475000);
    cycle = PRU1_CTRL.CYCLE;
    stall = PRU1_CTRL.STALL;
    pru_sram[0] = (float) cycle;
    __halt();
    */
    while(1){
        compFilter();       //IMPLEMENT MUTEX
        LQR();
    }
    
}

