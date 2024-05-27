#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "car_control.h"

#define CAR_ADDRESS 0x16

int fd;

void setup_i2c() {
    fd = wiringPiI2CSetup(CAR_ADDRESS);
    if (fd == -1) {
        printf("Failed to initialize I2C\n");
        exit(1);
    }
}

void write_block_data(int i2c_addr, unsigned char* data, int length) {
    for (int i = 0; i < length; ++i) {
        int result = wiringPiI2CWrite(fd, data[i]);
        if (result == -1) {
            printf("Failed to write data byte %d\n", i);
            exit(1);
        }
    }
}

void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed) {
    unsigned char data[] = { l_dir, l_speed, r_dir, r_speed };
    write_block_data(CAR_ADDRESS, data, 4);
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
    unsigned char data[] = { 0x02, 0x00 };
    write_block_data(CAR_ADDRESS, data, 2);
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
    unsigned char data[] = { id, angle };
    write_block_data(CAR_ADDRESS, data, 2);
}

int main() {
    setup_i2c();  // Initialize I2C only once

    // Example usage
    Car_Run(100, 100);
    delay(2000);  // Run for 2 seconds
    Car_Stop();

    return 0;
}
