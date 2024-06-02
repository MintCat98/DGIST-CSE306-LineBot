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

    VideoCapture camera;

    static set<string> detected_codes;

    bool is_new_code(const string& code) {
        if (detected_codes.find(code) == detected_codes.end()) {
            detected_codes.insert(code);
            return true;
        }
        return false;
    }

    void parseBarcodeData(const string& barcode_data, QRCodeInfo *qr_info) {
    if (barcode_data.length() == 2) {
        qr_info->x = barcode_data[0] - '0'; 
        qr_info->y = barcode_data[1] - '0'; 
    } else {
        // 잘못된 입력 처리
        cerr << "Invalid barcode data length" << endl;
        qr_info->x = 0;
        qr_info->y = 0;
    }
    }

    void camSetup() {
        camera(0);
        return;
    }

    void detectQRCode(QRCodeInfo *qr_info, bool *qr_detected) {
        if (!camera.isOpened()) {
            cerr << "Error: Unable to open the camera" << endl;
            return;
        }

        camera.set(CAP_PROP_FRAME_WIDTH, 320);
        camera.set(CAP_PROP_FRAME_HEIGHT, 240);
        camera.set(CAP_PROP_FPS, 30);
        camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
        camera.set(CAP_PROP_BRIGHTNESS, 40);
        camera.set(CAP_PROP_CONTRAST, 50);
        camera.set(CAP_PROP_EXPOSURE, 156);

        Mat frame;
        namedWindow("QR Code Detection", WINDOW_AUTOSIZE);

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
                vector<Point> vp;
                int n = symbol->get_location_size();
                for (int i = 0; i < n; i++) {
                    vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
                }
                RotatedRect r = minAreaRect(vp);
                Point2f pts[4];
                r.points(pts);
                for (int i = 0; i < 4; i++) {
                    line(frame, pts[i], pts[(i + 1) % 4], Scalar(225, 225, 225), 2);
                }

                string barcode_data = symbol->get_data();
                string barcode_type = symbol->get_type_name();

                putText(frame, barcode_data, Point(pts[1].x, pts[1].y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(225, 225, 225), 2);

                cout << "[INFO] Found " << barcode_type << " barcode: " << barcode_data << endl;

                if (1) {
                    parseBarcodeData(barcode_data.c_str(), qr_info);
                    strncpy(qr_info->data, barcode_data.c_str(), sizeof(qr_info->data) - 1);
                    qr_info->data[sizeof(qr_info->data) - 1] = '\0';
                    *qr_detected = true;
                    destroyAllWindows();  // QR 코드 인식을 한 번만 수행하고 종료
                    return;
                }
            }

            imshow("QR Code Detection", frame);

            if (waitKey(10) == 'q') {
                break;
            }
        }

        camera.release();
        destroyAllWindows();
    }
}
