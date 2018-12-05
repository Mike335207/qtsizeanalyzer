#ifndef QRASPICAMCONTROL_H
#define QRASPICAMCONTROL_H

#include <QObject>
#include "raspicam_cv.h"


class QRaspiCamControl
{
public:
    QRaspiCamControl();
    ~QRaspiCamControl();

    bool isInited() {return isCamInited;}

    cv::Mat getFrame();


private:
    raspicam::RaspiCam_Cv *camera = NULL;
    bool isCamInited = false;
};

#endif // QRASPICAMCONTROL_H
