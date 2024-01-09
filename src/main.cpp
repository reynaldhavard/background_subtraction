#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv4/opencv2/core/matx.hpp>
#include <vector>

class Gaussian {
private:
  double meanR;
  double meanG;
  double meanB;
  double variance;
  double weight;

public:
  Gaussian(){};
  Gaussian(double _meanR, double _meanG, double _meanB, double _variance,
           double _weight)
      : meanR(_meanR), meanG(_meanG), meanB(_meanB), variance(_variance),
        weight(_weight) {}

  double distance(cv::Vec3b &pixel) {
    double pixelR = static_cast<double>(pixel[0]);
    double pixelG = static_cast<double>(pixel[1]);
    double pixelB = static_cast<double>(pixel[2]);
    double dist = sqrt(1 / variance *
                       (pow(pixelR - meanR, 2) + pow(pixelG - meanG, 2) +
                        pow(pixelB - meanB, 2)));

    return dist;
  }

  double pdf(cv::Vec3b &pixel) {
    return pow(2 * M_PI * variance, -3.0 / 2.0) * exp(-distance(pixel) / 2.0);
  }

  bool isMatch(cv::Vec3b &pixel) {
    return (distance(pixel) <= 2.5 * sqrt(variance));
  }

  void reset(cv::Vec3b &pixel) {
    meanR = static_cast<double>(pixel[0]);
    meanG = static_cast<double>(pixel[1]);
    meanB = static_cast<double>(pixel[2]);
    variance = 100;
    weight = 0;
  }

  void adjust(cv::Vec3b &pixel, double lr) {
    double rho = lr * pdf(pixel);
    double pixelR = static_cast<double>(pixel[0]);
    double pixelG = static_cast<double>(pixel[1]);
    double pixelB = static_cast<double>(pixel[2]);
    meanR = (1 - rho) * meanR + rho * pixelR;
    meanG = (1 - rho) * meanG + rho * pixelG;
    meanB = (1 - rho) * meanB + rho * pixelB;
    variance = (1 - rho) * variance +
               rho * (pow(pixelR - meanR, 2) + pow(pixelG - meanG, 2) +
                      pow(pixelB - meanG, 2));
  }

  double getWeight() const { return weight; }

  void setWeight(double _weight) { weight = _weight; }

  double getWeightVarianceRatio() const { return weight / sqrt(variance); }
};

bool compareGaussian(const Gaussian &g1, const Gaussian &g2) {
  return g1.getWeightVarianceRatio() < g2.getWeightVarianceRatio();
}

class MOG {
private:
  int K;
  std::vector<Gaussian> gaussians;

public:
  MOG(){};
  MOG(int _K) : K(_K) {
    gaussians.resize(K);
    for (int k = 0; k < K; ++k) {
      gaussians[k] = Gaussian(-1, -1, -1, 1, 1.0 / K);
    }
  };

  void sortGaussians(int limit) {
    std::sort(gaussians.begin(), gaussians.begin() + limit + 1,
              compareGaussian);
  }

  int getNBackground(double T) {
    double sum = 0;
    int B = 1;
    for (int k = 0; k < K - 1; ++k) {
      sum += gaussians[k].getWeight();
      if (sum < T) {
        ++B;
      }
    }

    return B;
  }

  void adjustWeights(int modifiedIndex, double lr) {
    double sum = 0;
    for (int k = 0; k < K; ++k) {
      double newWeight = (1 - lr) * gaussians[k].getWeight();
      if (k == modifiedIndex) {
        newWeight += lr;
      }
      gaussians[k].setWeight(newWeight);
      sum += newWeight;
    }

    for (int k = 0; k < K; ++k) {
      gaussians[k].setWeight(gaussians[k].getWeight() / sum);
    }
  }

  bool update(cv::Vec3b &pixel, double lr, double T) {
    bool isForeground = false;

    int B = getNBackground(T);

    int matchIndex = K;
    for (int k = 0; k < K; ++k) {
      if (gaussians[k].isMatch(pixel)) {
        matchIndex = k;
        break;
      }
    }
    if (matchIndex > B) {
      isForeground = true;
    }
    if (matchIndex == K) {
      gaussians[K - 1].reset(pixel);
    } else {
      gaussians[matchIndex].adjust(pixel, lr);
    }
    int modifiedIndex = std::min(matchIndex, K - 1);
    adjustWeights(modifiedIndex, lr);
    sortGaussians(modifiedIndex);

    return isForeground;
  }
};

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

  double fps = cap.get(cv::CAP_PROP_FPS);
  int delay = static_cast<int>(1000 / fps);

  // Initialize Gaussians per pixel in matrix
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
          // fgPixel = pixel;
          fgPixel[0] = 255;
          fgPixel[1] = 255;
          fgPixel[2] = 255;
        }
      }
    }

    // Compare original frame and foreground
    cv::Mat displayMat;
    cv::hconcat(frame, foreground, displayMat);
    cv::imshow("display", displayMat);

    // int keyboard = cv::waitKey(delay);
    int keyboard = cv::waitKey(30);
    if (keyboard == 'q' || keyboard == 27) {
      break;
    }
  }

  cap.release();
  cv::destroyAllWindows();

  return 0;
}
