#include <stdio.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

int main() {
    setup();

    while (1) {
        tracking_function();
        printf("Tracking_func is called.\n");
        delay(100);
    }

    return 0;
}
