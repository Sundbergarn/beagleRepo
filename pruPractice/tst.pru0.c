#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

//THIS IS FROM PRU0's PERSPECTICE
#define PRU0_DRAM       0x00000
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

volatile unsigned int *pru0_dram = (unsigned int *) (PRU0_DRAM + 0x200);
volatile unsigned int *pru_sram = (unsigned int *) (PRU_SHAREDMEM);// + 0x200);

int main(){
    
    pru0_dram[0] = 2;
    pru_sram[0] = 10;

    return 0;
    
}