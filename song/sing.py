#!/usr/bin/env python3
import RPi.GPIO as GPIO
import time

Buzzer = 32  # Define the pin of the buzzer

GPIO.setwarnings(False)  # Ignore warning

# Define the frequency of each note
note_freqs = {
    '미': 329, '도': 262, '솔': 392, '(낮은)솔': 196, '라': 440, '시': 494, '(낮은)미':165, '(낮은)라':220,
     'b시': 233, '레': 294
}

# Define the melody from the image
tune1 = [
    '미', '미', '미', '도', '미',
    '솔', '(낮은)솔',
    # Add all other notes here
]

tune2 = [
    '도', '(낮은)솔', '(낮은)미',
    '(낮은)라', '(낮은)시', 'b시', '(낮은)라',
    '(낮은)솔', '미', '솔', '라', '파', '솔',
    '미', '도', '레','(낮은)시',
    # Add all other notes here
]

# Define the durations of each note (in seconds)
durt1 = [
    0.5, 1.0, 1.0, 0.5, 1.0,
    2.0, 2.0,
    # Add durations for all other notes here
]

durt2 = [
    1.5, 1.5, 1.5,
    1.0, 1.0, 0.5, 1.0,
    0.5, 0.5, 0.5, 1.5, 0.5, 1.5,
    0.5, 0.5, 0.5, 1.5,
    # Add durations for all other notes here
]

def setup():
    GPIO.setmode(GPIO.BOARD)      # Set the GPIO port to BOARD encoding mode
    GPIO.setup(Buzzer, GPIO.OUT)  # The pin of the buzzer is set to output mode
    global Buzz
    Buzz = GPIO.PWM(Buzzer, 440)
    Buzz.start(90)

def loop():
    print('\nPlaying song...')
    for i in range(len(tune1)):
        note = tune1[i]
        if note in note_freqs:
            Buzz.ChangeFrequency(note_freqs[note])
        time.sleep(durt1[i] * 0.5)
    while(1):
        for i in range(len(tune2)):
            note = tune2[i]
            if note in note_freqs:
                Buzz.ChangeFrequency(note_freqs[note])
            time.sleep(durt2[i] * 0.5)
    GPIO.cleanup()
    print("Finished")

if __name__ == '__main__':
    setup()
    loop()