#include <wiringPi.h>
#include <stdio.h>

// Define the GPIO pins for the sensors
#define SENSOR1_PIN 0 // GPIO 17 (WiringPi pin 0)
#define SENSOR2_PIN 1 // GPIO 18 (WiringPi pin 1)
#define SENSOR3_PIN 2 // GPIO 27 (WiringPi pin 2)
#define SENSOR4_PIN 3 // GPIO 22 (WiringPi pin 3)

// Define the GPIO pins for the motors
#define LEFT_MOTOR_FORWARD_PIN 4  // GPIO 23 (WiringPi pin 4)
#define LEFT_MOTOR_BACKWARD_PIN 5 // GPIO 24 (WiringPi pin 5)
#define RIGHT_MOTOR_FORWARD_PIN 6  // GPIO 25 (WiringPi pin 6)
#define RIGHT_MOTOR_BACKWARD_PIN 7 // GPIO 4 (WiringPi pin 7)

void setup() {
    // Initialize WiringPi
    wiringPiSetup();

    // Set sensor pins as input
    pinMode(SENSOR1_PIN, INPUT);
    pinMode(SENSOR2_PIN, INPUT);
    pinMode(SENSOR3_PIN, INPUT);
    pinMode(SENSOR4_PIN, INPUT);

    // Set motor pins as output
    pinMode(LEFT_MOTOR_FORWARD_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_BACKWARD_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_FORWARD_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_BACKWARD_PIN, OUTPUT);
}

void moveForward() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
}

void moveBackward() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, HIGH);
}

void turnLeft() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
}

void turnRight() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, HIGH);
}

void stop() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
}

int main(void) {
    setup();

    while (1) {
        int sensor1 = digitalRead(SENSOR1_PIN);
        int sensor2 = digitalRead(SENSOR2_PIN);
        int sensor3 = digitalRead(SENSOR3_PIN);
        int sensor4 = digitalRead(SENSOR4_PIN);

        if (sensor1 == LOW && sensor2 == LOW && sensor3 == LOW && sensor4 == LOW) {
            moveForward();
        }
        else if (sensor1 == HIGH && sensor2 == LOW && sensor3 == LOW && sensor4 == LOW) {
            turnLeft();
        }
        else if (sensor1 == LOW && sensor2 == LOW && sensor3 == LOW && sensor4 == HIGH) {
            turnRight();
        }
        else if (sensor1 == LOW && sensor2 == HIGH && sensor3 == LOW && sensor4 == LOW) {
            stop();
        }
    }

    return 0;
}
