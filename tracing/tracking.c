#include "YB_Pcb_Car.h"
#include <stdio.h>

int main() {
    YB_Pcb_Car car;

    printf("Car is running...\n");
    car.Car_Run(100, 100);  // Car run with speed 100 for both wheels
    delay(2000);            // Run for 2 seconds

    printf("Car is stopping...\n");
    car.Car_Stop();         // Stop the car

    printf("Car is backing...\n");
    car.Car_Back(50, 50);   // Car back with speed 50 for both wheels
    delay(2000);            // Back for 2 seconds

    printf("Car is turning left...\n");
    car.Car_Left(50, 100);  // Left turn with different speeds
    delay(2000);            // Turn for 2 seconds

    printf("Car is spinning right...\n");
    car.Car_Spin_Right(100, 100);  // Spin right with same speed
    delay(2000);                   // Spin for 2 seconds

    printf("Stopping all car movements.\n");
    car.Car_Stop();

    return 0;
}
