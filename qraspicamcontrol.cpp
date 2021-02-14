#include "qraspicamcontrol.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>

#define TIMEOUT 100

#define PATH_TO_IMG "/home/rapsodo/workspace/work/TechSys/build-QtSizeAnalyzer-Desktop_Qt_5_11_1_GCC_64bit-Release/images/2019-10-20-01-10-56.jpeg"

//using namespace raspicam;
using namespace std;
using namespace cv;

QRaspiCamControl::QRaspiCamControl()
{
    /*camera = new RaspiCam_Cv();

    qDebug()<<"Connecting to camera"<<endl;

    if ( !camera->open() ) qDebug()<<"Error opening camera" << endl;
    else
    {
        qDebug() <<"Connected to camera ="<< QString::fromStdString(camera->getId()) << endl;
        isCamInited = true;
    }*/

    camStream = new VideoCapture(1);

    if (!camStream->isOpened())
    { //check if video device has been initialised
        qDebug() << "cannot open camera";
    }else
    {
         qDebug() << "Camera opened!!!!";
    }

    camStream->set(cv::CAP_PROP_FRAME_WIDTH,1280);
    camStream->set(cv::CAP_PROP_FRAME_HEIGHT,800);

}

QRaspiCamControl::~QRaspiCamControl()
{
    delete camStream;
}

Mat QRaspiCamControl::getFrame()
{
    cv::Mat image;
    camStream->grab();
    camStream->retrieve(image);


    //camStream->read(image);

    //image = imread(PATH_TO_IMG);

    qDebug() << image.rows << "x" << image.cols;


    return image;
}
