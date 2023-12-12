#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char** argv){
    cv::VideoCapture capture("../data/vtest.avi");
    if (!capture.isOpened()){
        std::cerr << "Unable to open file!" << std::endl;
        return 0;
    }

    while(true){
        cv::Mat frame;
        capture >> frame;
        if (frame.empty()){
            break;
        }

        cv::imshow("frame", frame);

        int keyboard = cv::waitKey(30);
        if (keyboard == 'q' || keyboard == 27){
            break;
        }
    }

    capture.release();

    cv::destroyAllWindows();

    return 0;
}
