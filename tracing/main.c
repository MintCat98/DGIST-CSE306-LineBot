#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

int main() {
    setup();

    while (1) {
        int COMMAND = 1;    // command will be one of 0 ~ 3
        tracking_function(COMMAND);
        delay(100);
        
        int STOP_SIGNAL = stop_signal(/* need to be filled*/);    // stop signal will be 0(go) or 1(stop)
        while (STOP_SIGNAL == 0)
        {
            move_forward();
            delay(100);
        }
    }

    return 0;
}
