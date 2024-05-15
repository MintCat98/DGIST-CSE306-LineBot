#include "YB_Pcb_Car.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CAR_ADDRESS 0x16

YB_Pcb_Car::YB_Pcb_Car() {
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Setup wiringPi failed!\n");
        exit(1);
    }

    fd = wiringPiI2CSetup(CAR_ADDRESS);
    if (fd == -1) {
        fprintf(stderr, "Failed to init I2C communication.\n");
        exit(1);
    }
}

void write_u8(int reg, int data) {
    if (wiringPiI2CWriteReg8(fd, reg, data) < 0) {
        fprintf(stderr, "write_u8 I2C error\n");
    }
}

void write_array(int reg, uint8_t* data, int length) {
    for (int i = 0; i < length; i++) {
        if (wiringPiI2CWriteReg8(fd, reg + i, data[i]) < 0) {
            fprintf(stderr, "write_array I2C error\n");
            break;
        }
    }
}

void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed) {
    uint8_t data[] = {l_dir, l_speed, r_dir, r_speed};
    write_array(0x01, data, 4);
}

void Control_Car(int speed1, int speed2) {
    int dir1 = speed1 < 0 ? 0 : 1;
    int dir2 = speed2 < 0 ? 0 : 1;
    Ctrl_Car(dir1, abs(speed1), dir2, abs(speed2));
}

void Car_Run(int speed1, int speed2) {
    Ctrl_Car(1, speed1, 1, speed2);
}

void Car_Stop() {
    write_u8(0x02, 0x00);
}

void Car_Back(int speed1, int speed2) {
    Ctrl_Car(0, speed1, 0, speed2);
}

void Car_Left(int speed1, int speed2) {
    Ctrl_Car(0, speed1, 1, speed2);
}

void Car_Right(int speed1, int speed2) {
    Ctrl_Car(1, speed1, 0, speed2);
}

void Car_Spin_Left(int speed1, int speed2) {
    Ctrl_Car(0, speed1, 1, speed2);
}

void Car_Spin_Right(int speed1, int speed2) {
    Ctrl_Car(1, speed1, 0, speed2);
}

void Ctrl_Servo(int id, int angle) {
    if (angle < 0) angle = 0;
    else if (angle > 180) angle = 180;
    uint8_t data[] = {id, angle};
    write_array(0x03, data, 2);
}
