#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char **argv) {
  cv::VideoCapture cap("../data/vtest.avi");
  if (!cap.isOpened()) {
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  double fps = cap.get(cv::CAP_PROP_FPS);
  int delay = static_cast<int>(1000 / fps);

  while (true) {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
      break;
    }

    cv::Mat res = cv::Mat::zeros(frame.size(), frame.type());
    // Process

    // Compare original frame and foreground
    cv::Mat displayMat;
    cv::hconcat(frame, res, displayMat);
    cv::imshow("display", displayMat);

    int keyboard = cv::waitKey(delay);
    if (keyboard == 'q' || keyboard == 27) {
      break;
    }
  }

  cap.release();
  cv::destroyAllWindows();

  return 0;
}
