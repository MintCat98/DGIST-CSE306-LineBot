#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

int main() {
    setup();

    while (1) {
        tracking_function();
        delay(100);
    }

    return 0;
}
