#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>


#define PRU_ADDR        0x4A300000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

volatile float *pru_sram; // Points to the start of the shared memory


int main(){
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
    
    pru_sram[10] = 0;
    pru_sram[11] = 0;
    pru_sram[12] = 0;
    pru_sram[13] = 0;
    pru_sram[14] = 7900;
}