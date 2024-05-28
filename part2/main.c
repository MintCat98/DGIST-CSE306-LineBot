#include <stdio.h>

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
    void detectQRCode(struct QRCodeInfo *qr_info);
#ifdef __cplusplus
}
#endif

int main() {
    struct QRCodeInfo qr_info;
    detectQRCode(&qr_info);

    printf("QR Code detected at position (%d, %d) with data: %s\n", qr_info.x, qr_info.y, qr_info.data);

    return 0;
}