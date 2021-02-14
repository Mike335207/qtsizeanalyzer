#ifndef STRUCT_H
#define STRUCT_H

#include "opencv2/opencv.hpp"
#include <QString>

struct calibrationParameters
{
    cv::Rect bRect;
    cv::RotatedRect rRect;
    double pixelSizeInMilims = 0.0;
};

struct sizeItem
{
    cv::Point2f pnt1;
    cv::Point2f pnt2;
    int sizeInMilim = 0;
    int admissibleErrorInMilim = 0;
    QString sizeID = "";
};

#endif // STRUCT_H
