#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAXCH 4

#define PRU_ADDR        0x4A300000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU0_DRAM       0x00000         // Offset to DRAM
#define PRU1_DRAM       0x02000
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

unsigned int    *pru0DRAM_32int_ptr;        // Points to the start of local DRAM
unsigned int    *pru1DRAM_32int_ptr;        // Points to the start of local DRAM
unsigned int    *prusharedMem_32int_ptr;    // Points to the start of the shared memory




int main(){
 
    unsigned int    *pru;
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

    
    
    //"Skip the first 0x200 byte of DRAM since the Makefile allocates 0x100 for the STACK and 0x100 for the HEAP."
    // Convert 200 in Byte (8-base) to Hex (16-base) gives 0x80 (=0x200/4). Does this mean the abolsute address is in Bytes? Or Hex??
    
    pru0DRAM_32int_ptr = pru + PRU0_DRAM/4 + 0x200/4;                 // Points to 0x200 of PRU0 memory
    printf("PRU0-pointer points to: %p \n", pru0DRAM_32int_ptr);
    
    pru1DRAM_32int_ptr = pru + PRU1_DRAM/4 + 0x200/4;                 // Points to 0x200 of PRU1 memory
    printf("PRU1-pointer points to: %p \n", pru1DRAM_32int_ptr);
    
    prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4; //+ 0x200/4;                 // Points to 0x200 of PRU1 memory
    printf("PRUSHAREDMEM-pointer points to: %p \n\n", prusharedMem_32int_ptr);
    
    
    //pru1DRAM_32int_ptr[0]=70;
    printf("Val is: %d \n", pru0DRAM_32int_ptr[0]);
    printf("Val is: %d \n", pru1DRAM_32int_ptr[0]);
    printf("Val is: %d \n", prusharedMem_32int_ptr[0]);
    
    return 0;
}