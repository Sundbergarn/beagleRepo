#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

#define I2C2DATAREG     0x4819C09C      // Start of I2C2 R/W Data registers (Hopefully) 
#define PRU0_DRAM       0x00000         // Offset to DRAM
// Skip the first 0x200 byte of DRAM since the Makefile allocates
// 0x100 for the STACK and 0x100 for the HEAP.
volatile unsigned int *pru0_dram = (unsigned int *) (PRU0_DRAM + 0x200);
volatile unsigned int *I2C2RW = (unsigned int *) (I2C2DATAREG);


int main(){
    
    pru0_dram[0] =32;     // Copy to DRAM0 so the ARM can change it
    

    return 0;
    
}

