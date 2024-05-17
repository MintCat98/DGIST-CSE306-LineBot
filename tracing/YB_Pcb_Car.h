#ifndef YB_PCB_CAR_H
#define YB_PCB_CAR_H

#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

typedef struct {
    int fd;
} YB_Pcb_Car;

YB_Pcb_Car* YB_Pcb_Car_create();
void Control_Car(YB_Pcb_Car* car, int speed1, int speed2);
void Car_Run(YB_Pcb_Car* car, int speed1, int speed2);
void Car_Stop(YB_Pcb_Car* car);
void Car_Back(YB_Pcb_Car* car, int speed1, int speed2);
void Car_Left(YB_Pcb_Car* car, int speed1, int speed2);
void Car_Right(YB_Pcb_Car* car, int speed1, int speed2);
void Car_Spin_Left(YB_Pcb_Car* car, int speed1, int speed2);
void Car_Spin_Right(YB_Pcb_Car* car, int speed1, int speed2);
void Ctrl_Servo(YB_Pcb_Car* car, int id, int angle);
void write_u8(YB_Pcb_Car* car, int reg, int data);
void write_array(YB_Pcb_Car* car, int reg, uint8_t* data, int length);
void Ctrl_Car(YB_Pcb_Car* car, int l_dir, int l_speed, int r_dir, int r_speed);

#endif
