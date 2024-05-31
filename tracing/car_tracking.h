#ifndef TRACKING_FUNCTION_H
#define TRACKING_FUNCTION_H

#define Tracking_Left1 2 
#define Tracking_Left2 3
#define Tracking_Right1 0
#define Tracking_Right2 7

#define CAR_ADDRESS 0x16

void setup();

void move_forward(int Tracking_Left1Value, int Tracking_Left2Value, int Tracking_Right1Value, int Tracking_Right2Value);

void turn_left();

void turn_right();

int stop_signal(int left_sensor1, int right_sensor2);

int tracking_function(int command);

#endif
