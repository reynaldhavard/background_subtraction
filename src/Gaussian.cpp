#include "Gaussian.h"

double Gaussian::distance(cv::Vec3b &pixel) {
  const double pixelR = pixel[0];
  const double pixelG = pixel[1];
  const double pixelB = pixel[2];

  const double varInv = 1.0 / variance;
  double dist = ((pixelR - meanR) * (pixelR - meanR) +
                 (pixelB - meanB) * (pixelB - meanB) +
                 (pixelG - meanG) * (pixelG - meanG)) *
                varInv;

  return dist;
}

double Gaussian::pdf(cv::Vec3b &pixel) {
  const double dist = distance(pixel);

  return (1.0 / (variance * sqrt(2.0 * M_PI)) * exp(-dist / 2.0));
}

bool Gaussian::isMatch(cv::Vec3b &pixel) {
  const double threshold = 2.5 * 2.5 * variance;
  return distance(pixel) <= threshold;
}

void Gaussian::reset(cv::Vec3b &pixel) {
  meanR = static_cast<double>(pixel[0]);
  meanG = static_cast<double>(pixel[1]);
  meanB = static_cast<double>(pixel[2]);
  variance = 20;
}

void Gaussian::adjust(cv::Vec3b &pixel, double lr) {
  const double rho = lr * pdf(pixel);
  const double pixelR = pixel[0];
  const double pixelG = pixel[1];
  const double pixelB = pixel[2];

  meanR = (1 - rho) * meanR + rho * pixelR;
  meanG = (1 - rho) * meanG + rho * pixelG;
  meanB = (1 - rho) * meanB + rho * pixelB;

  const double diffR = pixelR - meanR;
  const double diffG = pixelG - meanG;
  const double diffB = pixelB - meanB;
  variance = (1 - rho) * variance +
             rho * (diffR * diffR + diffG * diffG + diffB * diffB);
}

bool compareGaussianDesc(const Gaussian &g1, const Gaussian &g2) {
  return g1.getWeightStdRatio() > g2.getWeightStdRatio();
}
