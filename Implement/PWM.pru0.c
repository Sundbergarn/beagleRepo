#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"
#include <math.h>
#define PRU0_DRAM       0x00000         // Offset to DRAM
#define PRU_SHAREDMEM   0x10000

volatile register uint32_t __R30;
volatile register uint32_t __R31;
uint32_t gpio[] = {P2_28, P2_30, P2_32, P2_34};
volatile float *pru_sram = (float*) (PRU_SHAREDMEM);

/*void init_Motors(){ //Initializes motors at 125-250 micro seconds intervall
    //__R30 &= 0xFFFF0000;				// Clear pins
    int i=0;
    while(1){
        __R30 |= (gpio[0]|gpio[1]|gpio[2]|gpio[3]);
        __delay_cycles(25000);
        __R30 &= ~(gpio[0]|gpio[1]|gpio[2]|gpio[3]);
        __delay_cycles(30000);    
        i++;
        if (i>20000) break;
    }
    i=0;
    while(1){
        __R30 |= (gpio[0]|gpio[1]|gpio[2]|gpio[3]);
        __delay_cycles(27000);
        __R30 &= ~(gpio[0]|gpio[1]|gpio[2]|gpio[3]);
        __delay_cycles(28000);    
        i++;
        if (i>40000) break;
    }
    
} */





int main(){
    int i;
    //init_Motors();
    volatile int M1, M2, M3, M4;
    while(1){
        i=0;
        M1 = (int) (pru_sram[14]+pru_sram[10]);
        M2 = (int) (pru_sram[14]+pru_sram[11]);
        M3 = (int) (pru_sram[14]+pru_sram[12]);
        M4 = (int) (pru_sram[14]+pru_sram[13]);
        __R30 |= (gpio[0]|gpio[1]|gpio[2]|gpio[3]);
        while(i<15000){
            if(i==M1) __R30 &= ~gpio[0];
            if(i==M2) __R30 &= ~gpio[1];
            if(i==M3) __R30 &= ~gpio[2];
            if(i==M4) __R30 &= ~gpio[3];
            i++;
        }
    }
    
    return 0;
}
