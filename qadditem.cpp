#include "qadditem.h"
#include "ui_qadditem.h"

#include "utils.h"
#include <QDebug>

using namespace cv;
using namespace std;

#define FPS 30



QAddItem::QAddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QAddItem)
{
    ui->setupUi(this);
    show();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(getFrame()));
    timer->start(1.0/(float)FPS);
}

QAddItem::~QAddItem()
{
    delete ui;
    delete timer;
}

void QAddItem::setFrame(cv::Mat &frame)
{
    ui->lb_liveStream->setFrame(frame);
    frame.copyTo(currentFrame);
    ui->pb_calibrate->setEnabled(true);
}

void QAddItem::setCalibParams(calibrationParameters _calibrParams)
{
    calibrParams = _calibrParams;
    if ((calibrParams.bRect.area()>0) && (calibrParams.pixelSizeInMilims > 0.0))
    {
        ui->pb_editItem->setEnabled(true);

        Mat tempFrame;
        currentFrame.copyTo(tempFrame);
        rectangle(tempFrame, calibrParams.bRect, Scalar(0, 0, 255), 3);
        Point2f globalROIVertexes[4];
        calibrParams.rRect.points(globalROIVertexes);

        for(int i = 0; i < 4; i++)
        {
            line(tempFrame, globalROIVertexes[i], globalROIVertexes[(i+1)%4], Scalar(0, 255, 0), 2);
        }

        ui->lb_liveStream->setFrame(tempFrame);
    }
}

void QAddItem::on_pb_getFrame_clicked()
{
    //emit requestFrame();
    timer->start(1.0/(float)FPS);
}

void QAddItem::on_pb_OK_clicked()
{
    close();
    timer->stop();
}

void QAddItem::on_pb_calibrate_clicked()
{
   timer->stop();
   emit calibrateCamera(currentFrame);
   emit saveImg(currentFrame);
}



void QAddItem::on_pb_editItem_clicked()
{
    if (dlgEditor != NULL)
    {
        delete dlgEditor;
        timer->stop();
    }

    Mat croppedImg = currentFrame(calibrParams.bRect);
    emit extractItemResend(croppedImg);


    dlgEditor = new QItemEditor(currentFrame, calibrParams, this);
    //connect(dlgEditor, SIGNAL(extractItem(Mat&)), this, SLOT(extractItemSlot(Mat&)));

    dlgEditor->show();

}

void QAddItem::extractItemSlot(Mat &img)
{
    emit extractItemResend(img);
}

void QAddItem::getFrame()
{
    emit requestFrame();
}
