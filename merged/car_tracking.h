#ifndef TRACKING_FUNCTION_H
#define TRACKING_FUNCTION_H

#define Tracking_Left1 2 
#define Tracking_Left2 3
#define Tracking_Right1 0
#define Tracking_Right2 7

#define CAR_ADDRESS 0x16

void setup();

void move_forward(int Tracking_Left1Value, int Tracking_Left2Value, int Tracking_Right1Value, int Tracking_Right2Value);

void move_left();

void move_right();

int intersection_signal(int l1, int l2, int r1, int r2);

void tracking_function();

extern int COMMAND;

#endif
