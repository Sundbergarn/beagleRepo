#include <cstdlib>
#include <cstdio>
//#include <stdexcept>
#include <cstring>
#include <string>
#include <iostream>
//#include <memory>
//#include <array>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;


const char* cmdGet = "i2cget -y 1";
const char* cmdSet = "i2cset -y 1";
const char* cd_Write00 = "cd /sys/class/pwm/pwm-0:0; echo ";
const char* cd_Write01 = "cd /sys/class/pwm/pwm-0:1; echo ";
const char* cd_Write20 = "cd /sys/class/pwm/pwm-2:0; echo ";
const char* cd_Write41 = "cd /sys/class/pwm/pwm-4:1; echo ";
const char* dutC = " > duty_cycle";


const char* deviceADDR = " 0x68";
const char* SLEEP = " 0x6B";
const char* ACC_X_H = " 0x3B ";
const char* ACC_X_L = " 0x3C ";
const char* ACC_Y_H = " 0x3D ";
const char* ACC_Y_L = " 0x3E ";
const char* ACC_Z_H = " 0x3F ";
const char* ACC_Z_L = " 0x40 ";
const char* GYRO_X_H = " 0x43 ";
const char* GYRO_X_L = " 0x44 ";
const char* GYRO_Y_H = " 0x45 ";
const char* GYRO_Y_L = " 0x46 ";
const char* GYRO_Z_H = " 0x47 ";
const char* GYRO_Z_L = " 0x48 ";

//Sensor readings
float acc_x;
float acc_y;
float acc_z;
float gyro_x;
float gyro_y;
float gyro_z;

//Complementary filter params and estimates
const float h = 0.01; //SUBJECT TO CHANGE --- CAN WE ENSURE APPROX 0.001?
const float gamma_Comp = 0.95;
float roll_est = 0;
float pitch_est = 0;
float yawRate_est = 0;

//Mutex-lock attempt at interval
std::chrono::microseconds mxInterval(100);
std::mutex mx;


string execBash(char* cmd){
	string res;
	FILE *stream;
	char buffer[5];
	stream = popen(cmd,"r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, 5, stream) != NULL)
				res.append(buffer);
		pclose(stream);	
	}

	return res;
}

string get(const char* reg1, const char* reg2){
	char str[100];
	string str2;
 
	strcpy(str, cmdGet);
	strcat(str, reg1);
	strcat(str, reg2);
 
	str2 = execBash(str);
	return str2;
}



void writePWM(int motor,int pwm){

	char charArr[100];
    if (motor==1) strcpy(charArr,cd_Write00);
    if (motor==2) strcpy(charArr,cd_Write01);
    if (motor==3) strcpy(charArr,cd_Write20);
    if (motor==4) strcpy(charArr,cd_Write41);
    
    std::string tmp = std::to_string(pwm);
    const char* pwm_char = tmp.c_str();
    strcat(charArr, pwm_char);
    strcat(charArr, dutC);
    
    execBash(charArr);
    
}

void compFilter(){   //Well it's definitely not working
	
	while(true){
		if(mx.try_lock()){
			float accRollEst = atan2f(acc_y,acc_z);  // Guessing Roll etc.
			float accPitchEst = atan2f(-acc_x, (float) abs(acc_y+acc_z)); 	//Not sure float cast needed, they are floats
			float gyroRollEst = h*gyro_x;
			float gyroPitchEst = h*gyro_y;
			float gyroYawEst = h*gyro_z; //Not sure about yaw estimate through this
	
			roll_est = (1-gamma_Comp)*accRollEst + gamma_Comp*(gyroRollEst+roll_est);
			pitch_est = (1-gamma_Comp)*accPitchEst + gamma_Comp*(gyroPitchEst+pitch_est);
			yawRate_est = gamma_Comp*(gyroYawEst+yawRate_est);
			mx.unlock();
			return;
		} else std::this_thread::sleep_for(mxInterval);
	}
	
}


void readMPU6050(){

		while(true){
			if(mx.try_lock()){
				int16_t acc_xH = stoi(get(deviceADDR,ACC_X_H), nullptr, 16) << 8;
    			int16_t acc_xL = stoi(get(deviceADDR, ACC_X_L), nullptr, 16);
   				acc_x = (float(acc_xH+acc_xL))/16384;
	
				int16_t acc_yH = stoi(get(deviceADDR,ACC_Y_H), nullptr, 16) << 8;
				int16_t acc_yL = stoi(get(deviceADDR, ACC_Y_L), nullptr, 16);	
				acc_y = -(float(acc_yH + acc_yL))/16384;

				int16_t acc_zH = stoi(get(deviceADDR,ACC_Z_H), nullptr, 16) << 8;
    			int16_t acc_zL = stoi(get(deviceADDR, ACC_Z_L), nullptr, 16);
    			acc_z = -(float(acc_zH + acc_zL))/16384;

	
				int16_t gyro_xH = stoi(get(deviceADDR,GYRO_X_H), nullptr, 16) << 8;
    			int16_t gyro_xL = stoi(get(deviceADDR, GYRO_X_L), nullptr, 16);
    			gyro_x = (float(gyro_xH + gyro_xL))/131;

				int16_t gyro_yH = stoi(get(deviceADDR,GYRO_Y_H), nullptr, 16) << 8;
    			int16_t gyro_yL = stoi(get(deviceADDR, GYRO_Y_L), nullptr, 16);
			  	gyro_y = -(float(gyro_yH + gyro_yL))/131;

				int16_t gyro_zH = stoi(get(deviceADDR,GYRO_Z_H), nullptr, 16) << 8;
    			int16_t gyro_zL = stoi(get(deviceADDR, GYRO_Z_L), nullptr, 16);
				gyro_z = -(float(gyro_zH + gyro_zL))/131;
		
				mx.unlock();
				return;
			}else std::this_thread::sleep_for(mxInterval);
		}
}

int main() {
	
	using namespace std::chrono;
	
	auto next = steady_clock::now();
	for(int i=1;i<100;i++){

		//int16_t pwm = int(acc_z*1000);
		//writePWM(1,pwm);
		readMPU6050();
		compFilter();
		
		//cout << "\n\n";
		//cout << roll_est << endl;
		//cout << pitch_est << endl;
		
	
		
		/*cout << acc_x << endl;
		cout << acc_y << endl;
		cout << acc_z << endl;
		cout << "\n\n";
		cout << "\t" << gyro_x << endl;
		cout << "\t" << gyro_y << endl;
		cout << "\t" << gyro_z << endl;
		cout << "\n\n"; */
	
		next += 1ms;
		std::this_thread::sleep_until(next);	
	}
	
		
return 0;
}
