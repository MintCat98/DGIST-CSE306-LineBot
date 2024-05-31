#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace cv;
using namespace std;
using namespace zbar;

extern "C" {
    struct QRCodeInfo {
        int x;
        int y;
        char data[128];
    };

    static set<string> detected_codes;

    bool is_new_code(const string& code) {
        if (detected_codes.find(code) == detected_codes.end()) {
            detected_codes.insert(code);
            return true;
        }
        return false;
    }

    void detectQRCode(QRCodeInfo *qr_info, bool *qr_detected) {
        VideoCapture camera(0);
        if (!camera.isOpened()) {
            cerr << "Error: Unable to open the camera" << endl;
            return;
        }
        camera.set(CAP_PROP_FRAME_WIDTH, 320);
        camera.set(CAP_PROP_FRAME_HEIGHT, 240);
        camera.set(CAP_PROP_FPS, 30);
//

        Mat frame;
// 카메라 설정 및 프레임 처리 최적화 , 디버깅 출력지움
// 프레임 그레이스케일로 변환해서 더 빨리 qr 인식 
        ImageScanner scanner;
        scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

        while (true) {
            camera >> frame;
            if (frame.empty()) {
                cerr << "Error: Blank frame grabbed" << endl;
                break;
            }
            Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            int width = gray.cols;
            int height = gray.rows;
            uchar *raw = (uchar *)(gray.data);
            Image imageZBar(width, height, "Y800", raw, width * height);
            scanner.scan(imageZBar);

            for (Image::SymbolIterator symbol = imageZBar.symbol_begin(); symbol != imageZBar.symbol_end(); ++symbol) {
                string qr_data = symbol->get_data();
                if (is_new_code(qr_data)) {
                    int x = qr_data[0] - '0';
                    int y = qr_data[1] - '0';
                    qr_info->x = x;
                    qr_info->y = y;
                    strncpy(qr_info->data, qr_data.c_str(), sizeof(qr_info->data));
                    *qr_detected = true;
                    return;
                }
            }
            *qr_detected = false;
                
            }
        camera.release();
        destroyAllWindows();
    }
}
