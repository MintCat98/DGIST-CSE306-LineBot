#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <stdint.h>

#define PIN_L1 2 
#define PIN_L2 3
#define PIN_R1 0
#define PIN_R2 7

#define CAR_ADDRESS 0x16

extern int fd;

void write_block_data(int i2c_addr, unsigned char* data, int length);

void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed);

void Control_Car(int speed1, int speed2);

void Car_Run(int speed1, int speed2);

void Car_Stop();

void Car_Back(int speed1, int speed2);

void Car_Left(int speed1, int speed2);

void Car_Right(int speed1, int speed2);

void Car_Spin_Left(int speed1, int speed2);

void Car_Spin_Right(int speed1, int speed2);

void Ctrl_Servo(int id, int angle);

#endif // CAR_CONTROL_H
