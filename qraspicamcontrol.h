#ifndef QRASPICAMCONTROL_H
#define QRASPICAMCONTROL_H

#include <QObject>
//#include "raspicam_cv.h"
#include "opencv2/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class QRaspiCamControl
{
public:
    QRaspiCamControl();
    ~QRaspiCamControl();

    bool isInited() {return isCamInited;}

    cv::Mat getFrame();


private:
    //raspicam::RaspiCam_Cv *camera = NULL;
    bool isCamInited = false;
    cv::VideoCapture*  camStream;
};

#endif // QRASPICAMCONTROL_H
