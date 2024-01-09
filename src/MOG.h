#pragma once

#include "Gaussian.h"
#include <vector>

class MOG {
private:
  int K;
  std::vector<Gaussian> gaussians;

public:
  MOG(){};
  MOG(int _K);

  void sortGaussians(int limit);

  int getBackgroundLimit(double T);

  void adjustWeights(int modifiedIndex, double lr);

  bool update(cv::Vec3b &pixel, double lr, double T);
};
