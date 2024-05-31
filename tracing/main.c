#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

/*
    나중에 진짜 main.c 만들 때 어디에서 global COMMAND 선언할 건지 결정하기
    decide_movement() 함수를 통해 globally update될 거임!!!
*/
int COMMAND = 0;

int main() {
    setup();
    tracking_function();
    
    return 0;
}