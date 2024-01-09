#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <opencv2/core.hpp>

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

  double distance(cv::Vec3b &pixel);

  double pdf(cv::Vec3b &pixel);

  bool isMatch(cv::Vec3b &pixel);

  void reset(cv::Vec3b &pixel);

  void adjust(cv::Vec3b &pixel, double lr);

  double getWeight() const { return weight; }

  void setWeight(double _weight) { weight = _weight; }

  double getWeightStdRatio() const { return weight / sqrt(variance); }
};

bool compareGaussianDesc(const Gaussian &g1, const Gaussian &g2);
