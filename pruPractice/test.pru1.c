#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

//THIS IS FROM PRU1's PERSPECTICE
#define PRU1_DRAM       0x00000         // Absolute addr == 0x02000
#define PRU_SHAREDMEM   0x10000          // Offset to shared memory from PRU 1, Absolute addr == 0x10000

volatile unsigned int *pru1_dram = (unsigned int *) (PRU1_DRAM + 0x200);
volatile unsigned int *pru_sram = (unsigned int *) (PRU_SHAREDMEM); 

int main(){
    
    pru1_dram[0] = 11;
    pru_sram[0] = 12;

    return 0;
    
}
