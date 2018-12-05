#include "qraspicamcontrol.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>

#define TIMEOUT 100

using namespace raspicam;
using namespace std;
using namespace cv;

QRaspiCamControl::QRaspiCamControl()
{
    camera = new RaspiCam_Cv();

    qDebug()<<"Connecting to camera"<<endl;

    if ( !camera->open() ) qDebug()<<"Error opening camera" << endl;
    else
    {
        qDebug() <<"Connected to camera ="<< QString::fromStdString(camera->getId()) << endl;
        isCamInited = true;
    }

}

QRaspiCamControl::~QRaspiCamControl()
{
    camera->release();
    delete camera;
}

Mat QRaspiCamControl::getFrame()
{
    cv::Mat image;
    camera->grab();
    camera->retrieve(image);

    return image;
}
