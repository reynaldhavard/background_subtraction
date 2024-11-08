#include "MOG.h"

MOG::MOG(int _K) : K(_K) {
  gaussians.resize(K);
  for (int k = 0; k < K; ++k) {
    gaussians[k] = Gaussian(-1, -1, -1, 10, 1.0 / K);
  }
}

void MOG::sortGaussians(int limit) {
  std::sort(gaussians.begin(), gaussians.begin() + limit + 1,
            compareGaussianDesc);
}

int MOG::getBackgroundLimit(double T) {
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

void MOG::adjustWeights(int modifiedIndex, double lr) {
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

bool MOG::update(cv::Vec3b &pixel, double lr, double T) {

  int B = getBackgroundLimit(T);

  int matchIndex = K;
  for (int k = 0; k < K; ++k) {
    if (gaussians[k].isMatch(pixel)) {
      matchIndex = k;
      break;
    }
  }
  const bool isForeground = (matchIndex > B);
  if (matchIndex == K) {
    gaussians[K - 1].reset(pixel);
  } else {
    gaussians[matchIndex].adjust(pixel, lr);
  }
  const int modifiedIndex = std::min(matchIndex, K - 1);
  adjustWeights(modifiedIndex, lr);
  sortGaussians(modifiedIndex);

  return isForeground;
}
