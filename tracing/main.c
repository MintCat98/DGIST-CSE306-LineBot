#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

int main() {
    setup();

    while (1) {
        //전진함
        move_forward();
        int command = get_command(); //서버에서 받아온 command 받음

        //command가 1이면 멈추지 않고 직진, 2,3이면 좌,우회전
        if (stop_signal(digitalRead(Tracking_Left1),digitalRead(Tracking_Right2))){
            if (command != 1) {
                Run_Car(0,0);
                delay(50);
                if (command == 2) {
                    turn_left();
                } else if (command == 3) {
                turn_right();
            }
        }
        delay(100);
    }
    return 0;
}
