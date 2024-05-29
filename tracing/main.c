#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

int main() {
    setup();

    while (1) {
        int COMMAND = 1;    // command will be one of 0 ~ 3
        int STOP_SIGNAL = tracking_function(COMMAND);
        delay(100);
        
        while (STOP_SIGNAL == 0)
        {
            move_forward();
            delay(100);
        }
    }
    
    return 0;
}
