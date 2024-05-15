#ifndef YB_PCB_CAR_H
#define YB_PCB_CAR_H

#include <wiringPi.h>
#include <wiringPiI2C.h>

class YB_Pcb_Car {
public:
    YB_Pcb_Car();
    void Control_Car(int speed1, int speed2);
    void Car_Run(int speed1, int speed2);
    void Car_Stop();
    void Car_Back(int speed1, int speed2);
    void Car_Left(int speed1, int speed2);
    void Car_Right(int speed1, int speed2);
    void Car_Spin_Left(int speed1, int speed2);
    void Car_Spin_Right(int speed1, int speed2);
    void Ctrl_Servo(int id, int angle);
    int fd;
    void write_u8(int reg, int data);
    void write_array(int reg, uint8_t* data, int length);
    void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed);
};

#endif