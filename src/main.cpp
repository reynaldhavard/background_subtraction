#include "MOG.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  int K;
  double lr;
  double T;
  if (argc == 4) {
    K = std::stoi(argv[1]);
    lr = std::stod(argv[2]);
    T = std::stod(argv[3]);
  } else {
    std::cerr << "Use as \"./backgroundSubtraction K lr T\"" << std::endl;
    return 0;
  }

  cv::VideoCapture cap("../data/vtest.avi");
  if (!cap.isOpened()) {
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);

  std::vector<std::vector<MOG>> mogVector(height, std::vector<MOG>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      mogVector[i][j] = MOG(K);
    }
  }

  while (true) {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
      break;
    }

    cv::Mat foreground = cv::Mat::zeros(frame.size(), frame.type());

    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        cv::Vec3b &pixel = frame.at<cv::Vec3b>(i, j);
        if (mogVector[i][j].update(pixel, lr, T)) {
          cv::Vec3b &fgPixel = foreground.at<cv::Vec3b>(i, j);
          fgPixel = pixel;
        }
      }
    }

    // Compare original frame and foreground
    cv::Mat displayMat;
    cv::hconcat(frame, foreground, displayMat);
    cv::imshow("display", displayMat);

    int keyboard = cv::waitKey(10);
    if (keyboard == 'q' || keyboard == 27) {
      break;
    }
  }

  cap.release();
  cv::destroyAllWindows();

  return 0;
}
