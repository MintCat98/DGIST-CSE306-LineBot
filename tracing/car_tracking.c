#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"

// GPIO 핀 정의
#define Tracking_Left1 0
#define Tracking_Left2 1
#define Tracking_Right1 2
#define Tracking_Right2 3

void setup() {
    // WiringPi 초기화
    wiringPiSetup();

    // GPIO 핀 입력 모드로 설정
    pinMode(Tracking_Left1, INPUT);
    pinMode(Tracking_Left2, INPUT);
    pinMode(Tracking_Right1, INPUT);
    pinMode(Tracking_Right2, INPUT);
}

void tracking_function() {
    // GPIO 입력 값 읽기
    int Tracking_Left1Value = digitalRead(Tracking_Left1);
    int Tracking_Left2Value = digitalRead(Tracking_Left2);
    int Tracking_Right1Value = digitalRead(Tracking_Right1);
    int Tracking_Right2Value = digitalRead(Tracking_Right2);

    // 네 개의 추적 핀 상태
    // 0 0 X 0
    // 1 0 X 0
    // 0 1 X 0
    // 위의 세 가지 상태에서는 차량이 오른쪽으로 회전합니다.
    // 오른쪽 급각 및 오른쪽 직각 회전 처리
    if ((Tracking_Left1Value == LOW || Tracking_Left2Value == LOW) && Tracking_Right2Value == LOW) {
        Ctrl_Car(1, 70, 0, 30);
        delay(200); // 0.2초 대기
    }
    // 네 개의 추적 핀 상태
    // 0 X 0 0
    // 0 X 0 1
    // 0 X 1 0
    // 왼쪽 급각 및 왼쪽 직각 회전 처리
    else if (Tracking_Left1Value == LOW && (Tracking_Right1Value == LOW || Tracking_Right2Value == LOW)) {
        Ctrl_Car(0, 30, 1, 70);
        delay(200); // 0.2초 대기
    }
    // 가장 왼쪽이 감지됨
    else if (Tracking_Left1Value == LOW) {
        Ctrl_Car(0, 70, 1, 70);
        delay(50); // 0.05초 대기
    }
    // 가장 오른쪽이 감지됨
    else if (Tracking_Right2Value == LOW) {
        Ctrl_Car(1, 70, 0, 70);
        delay(50); // 0.05초 대기
    }
    // 왼쪽 작은 커브 처리
    else if (Tracking_Left2Value == LOW && Tracking_Right1Value == HIGH) {
        Ctrl_Car(0, 60, 1, 60);
        delay(20); // 0.02초 대기
    }
    // 오른쪽 작은 커브 처리
    else if (Tracking_Left2Value == HIGH && Tracking_Right1Value == LOW) {
        Ctrl_Car(1, 60, 0, 60);
        delay(20); // 0.02초 대기
    }
    // 직선 처리
    else if (Tracking_Left2Value == LOW && Tracking_Right1Value == LOW) {
        Ctrl_Car(1, 70, 1, 70);
    }
    // 모든 핀 값이 1인 경우 이전 상태 유지
}