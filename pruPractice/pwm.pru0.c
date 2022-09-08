#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;


void main(void)
{
    uint32_t gpio = P2_34;  // Select which pin to toggle.;

    /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    int i=0;
    while(1) {
        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(20000);
        __R30 &= ~gpio;     // Clear the GPIO pin
        __delay_cycles(60000-20000); 
    
        i++;
        if(i>1000000) break;
    }
}
