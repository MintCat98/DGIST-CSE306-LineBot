#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// QR 코드 데이터 구조체 정의 (C에서 동일하게 사용)
struct QRCodeInfo {
    int x;
    int y;
    char data[128];
};

// C++에서 정의된 함수 선언
#ifdef __cplusplus
extern "C" {
#endif
    void detectQRCode(struct QRCodeInfo *qr_info, bool *qr_detected);
#ifdef __cplusplus
}
#endif

int main() {
    struct QRCodeInfo qr_info;
    bool qr_detected = false;

    while (true) {
        detectQRCode(&qr_info, &qr_detected);
        
        if (qr_detected) {
            printf("Current location: (%d, %d)\n", qr_info.x, qr_info.y);
            printf("QR Code Data: %s\n", qr_info.data);
            qr_detected = false; // Reset the flag for the next detection
        }
    }

    return 0;
}
