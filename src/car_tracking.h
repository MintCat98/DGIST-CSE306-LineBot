#ifndef TRACKING_FUNCTION_H
#define TRACKING_FUNCTION_H

#define Tracking_Left1 2 
#define Tracking_Left2 3
#define Tracking_Right1 0
#define Tracking_Right2 7

#define CAR_ADDRESS 0x16

#include <signal.h>

void setup();

void move_forward();

void move_left();

void move_right();

int intersection_signal(int l1, int l2, int r1, int r2);

void tracking_function();

extern int COMMAND;
extern volatile sig_atomic_t stop;

#endif
