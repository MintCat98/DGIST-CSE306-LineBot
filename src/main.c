#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

int COMMAND = 0;

int main() {
    setup();
    tracking_function();
    
    return 0;
}
