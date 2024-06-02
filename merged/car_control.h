#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <stdint.h>


extern int fd;

void write_block_data(int i2c_addr, unsigned char* data, int length);

void Ctrl_Car(int l_dir, int l_speed, int r_dir, int r_speed);

void Run_Car(int speed1, int speed2);

#endif // CAR_CONTROL_H
