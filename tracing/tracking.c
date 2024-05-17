#include "YB_Pcb_Car.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
    YB_Pcb_Car* car = YB_Pcb_Car_create();

    printf("Car is running...\n");
    Car_Run(car, 100, 100); // Car run with speed 100 for both wheels
    delay(2000); // Run for 2 seconds

    printf("Car is stopping...\n");
    Car_Stop(car); // Stop the car

    printf("Car is backing...\n");
    Car_Back(car, 50, 50); // Car back with speed 50 for both wheels
    delay(2000); // Back for 2 seconds

    printf("Car is turning left...\n");
    Car_Left(car, 50, 100); // Left turn with different speeds
    delay(2000); // Turn for 2 seconds

    printf("Car is spinning right...\n");
    Car_Spin_Right(car, 100, 100); // Spin right with same speed
    delay(2000); // Spin for 2 seconds

    printf("Stopping all car movements.\n");
    Car_Stop(car);

    free(car); // Free the allocated memory
    return 0;
}
