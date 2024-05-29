#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace zbar;

void decodeDisplay(Mat &image) {
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

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
            line(image, pts[i], pts[(i + 1) % 4], Scalar(225, 225, 225), 2);
        }

        string barcodeData = symbol->get_data();
        string barcodeType = symbol->get_type_name();

        putText(image, barcodeData + " (" + barcodeType + ")", Point(pts[1].x, pts[1].y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(225, 225, 225), 2);

        cout << "[INFO] Found " << barcodeType << " barcode: " << barcodeData << endl;
    }
}

void detect() {
    VideoCapture camera(0);
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
    namedWindow("Barcode Detection", WINDOW_AUTOSIZE);

    try {
        while (true) {
            camera >> frame;
            if (frame.empty()) {
                cerr << "Error: Blank frame grabbed" << endl;
                break;
            }

            decodeDisplay(frame);
            imshow("Barcode Detection", frame);

            if (waitKey(10) == 'q') {
                break;
            }
        }
    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }

    camera.release();
    destroyAllWindows();
}

int main() {
    detect();
    return 0;
}
