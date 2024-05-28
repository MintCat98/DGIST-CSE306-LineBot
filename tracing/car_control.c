#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include "car_control.h"

int fd;

void write_block_data(int i2c_addr, unsigned char* data, int length) {
    int new_len = length + 1;
    unsigned char copy[new_len];
    copy[0] = i2c_addr;
    for (size_t i = 0; i < length; i++)
    {
        copy[i + 1] = data[i];
    }
    write(fd, copy, new_len);
    if (fd == -1) {
        printf("Failed to write!\n");
        exit(1);
    }
    printf("Data-Write Success.\n");
}

void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed) {
    int i2c_addr;
    unsigned char data[] = { l_dir, l_speed, r_dir, r_speed };
    write_block_data(i2c_addr, data, 4);
}

void Run_Car(int speed1, int speed2) {
    int dir1 = speed1 < 0 ? 0 : 1;
    int dir2 = speed2 < 0 ? 0 : 1;
    Ctrl_Car(dir1, abs(speed1), dir2, abs(speed2));
}
