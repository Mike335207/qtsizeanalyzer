#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <vector>

void findSquares(const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares);
void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares);




#endif // UTILS_H
