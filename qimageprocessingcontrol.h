#ifndef QIMAGEPROCESSINGCONTROL_H
#define QIMAGEPROCESSINGCONTROL_H

#include <QObject>
#include <vector>
#include "opencv2/core.hpp"
#include "struct.h"


class QImageProcessingControl : public QObject
{
    Q_OBJECT
public:
    explicit QImageProcessingControl(QObject *parent = nullptr);
    bool calibrateCamera(cv::Mat frame);
    calibrationParameters getCalibrParams() {return calibrParams;}
    void performItemExtraction(cv::Mat croppedFrame, std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Vec4i>& hierarchy, int& largestContourID);
    double getOrientation(std::vector<cv::Point> &pts, cv::Mat &img);
    void rotateImage(cv::Mat &img, double angle, cv::Point2f center);

signals:
    void extractItem(cv::Mat &croppedFrame);

public slots:


private:
    bool isCamCalibrated = false;
    calibrationParameters calibrParams;

};

#endif // QIMAGEPROCESSINGCONTROL_H
