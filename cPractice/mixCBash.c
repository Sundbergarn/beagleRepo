#include <stdio.h>
#include <stdlib.h>



#define SHELLS "\
#!/bin/bash \n\ 
i2cget -y 1 0x68 0x3B \n\
"
char accX;
int main(){

	//for(int i=0;i<3;i++){
	
	accX = system(SHELLS);
	//}
	printf("accX is: %c \n", accX);
	return 0;
} 
