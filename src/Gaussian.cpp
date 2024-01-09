#include "Gaussian.h"

double Gaussian::distance(cv::Vec3b &pixel) {
  double pixelR = static_cast<double>(pixel[0]);
  double pixelG = static_cast<double>(pixel[1]);
  double pixelB = static_cast<double>(pixel[2]);
  double dist = sqrt(1 / variance *
                     (pow(pixelR - meanR, 2) + pow(pixelG - meanG, 2) +
                      pow(pixelB - meanB, 2)));

  return dist;
}

double Gaussian::pdf(cv::Vec3b &pixel) {
  return pow(2 * M_PI * variance, -1.0 / 2.0) *
         exp(-pow(distance(pixel), 2.0) / 2.0);
}

bool Gaussian::isMatch(cv::Vec3b &pixel) {
  return (distance(pixel) <= 2.5 * sqrt(variance));
}

void Gaussian::reset(cv::Vec3b &pixel) {
  meanR = static_cast<double>(pixel[0]);
  meanG = static_cast<double>(pixel[1]);
  meanB = static_cast<double>(pixel[2]);
  variance = 20;
}

void Gaussian::adjust(cv::Vec3b &pixel, double lr) {
  double rho = lr * pdf(pixel);
  double pixelR = static_cast<double>(pixel[0]);
  double pixelG = static_cast<double>(pixel[1]);
  double pixelB = static_cast<double>(pixel[2]);
  meanR = (1 - rho) * meanR + rho * pixelR;
  meanG = (1 - rho) * meanG + rho * pixelG;
  meanB = (1 - rho) * meanB + rho * pixelB;
  variance = (1 - rho) * variance +
             rho * (pow(pixelR - meanR, 2) + pow(pixelG - meanG, 2) +
                    pow(pixelB - meanB, 2));
}

bool compareGaussianDesc(const Gaussian &g1, const Gaussian &g2) {
  return g1.getWeightStdRatio() > g2.getWeightStdRatio();
}
