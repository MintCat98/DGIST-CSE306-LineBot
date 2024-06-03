#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include "car_control.h"

int fd;

void write_block_data(int reg, unsigned char* data, int length) {
    int new_len = length + 1;
    unsigned char copy[new_len];
    copy[0] = reg;
    for (size_t i = 0; i < length; i++)
    {
        copy[i + 1] = data[i];
    }
    write(fd, copy, new_len);
    if (fd == -1) {
        printf("Failed to write!\n");
        exit(1);
    }
}

void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed) {
    int reg = 0x01;
    unsigned char data[] = { l_dir, l_speed, r_dir, r_speed };
    write_block_data(reg, data, 4);
}

void Run_Car(int speed1, int speed2) {
    int dir1 = speed1 < 0 ? 0 : 1;
    int dir2 = speed2 < 0 ? 0 : 1;
    Ctrl_Car(dir1, abs(speed1), dir2, abs(speed2));
}

void Stop_Car() {
    int reg = 0x01;
    unsigned char data[] = { 0, 0, 0, 0 };
    write_block_data(reg, data, 4);
    delay(20);
}
