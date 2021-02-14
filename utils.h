#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <vector>

double calculateDistanceBtw2Pnts(cv::Point2f pnt1, cv::Point2f pnt2);

void findSquares(const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares);
void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares);

std::vector<cv::RotatedRect> detectSquares(cv::Mat& input);
double calculatePixelSizeInMilimeters(cv::RotatedRect box, int squareSideInMilimeters);




#endif // UTILS_H
