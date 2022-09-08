#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

const char* cd_Write00 = "cd /sys/class/pwm/pwm-0:0; echo ";
const char* dutC = " > duty_cycle";


string execBash(char* cmd){
	string res;
	FILE *stream;
	char buffer[256];
	stream = popen(cmd,"r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, 256, stream) != NULL)
				res.append(buffer);
		pclose(stream);	
	}

	return res;
}


int main(){
    
    char writePWM[100];
    int16_t pwm = 300;
    std::string tmp = std::to_string(pwm);
    const char* pwm_char = tmp.c_str();
    
    
    strcpy(writePWM, cd_Write00);
    strcat(writePWM, pwm_char);
    strcat(writePWM, dutC);
    //execBash(writePWM);    
    
    cout << writePWM << endl;
    return 0;
}