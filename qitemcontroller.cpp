#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>


#include "qitemcontroller.h"
#include "ui_qitemcontroller.h"

using namespace cv;

//#define PATH_TO_IMG "/home/rapsodo/workspace/work/TechSys/qtsizeanalyzer/resources/index.jpeg"
#define FPS 30
#define MAX_ERROR_RADIANS 0.15


QItemController::QItemController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QItemController)
{
    ui->setupUi(this);
   imgUtils = new QImageProcessingControl();

   timer = new QTimer();
   connect(timer, SIGNAL(timeout()), this, SLOT(getFrame()));
   timer->start(1.0/(float)FPS);
}

QItemController::~QItemController()
{
    delete ui;
    delete timer;
}

void QItemController::on_pb_loadModel_clicked()
{
    timer->stop();
    QString fileName = QFileDialog::getOpenFileName(this,
           tr("Open Item's Model"), "",
           tr("Item models (*.mod);;All Files (*)"));

    if (fileName.isEmpty())
    {
        ui->lbl_modelName->setText("EMPTY FILE");
        ui->lbl_modelName->setStyleSheet("QLabel {color : red; }");

    } else
    {
           ui->lbl_modelName->setText("CURRENT MODEL: " + fileName);
           ui->lbl_modelName->setStyleSheet("QLabel {color : green; }");

           QFile file(fileName);

           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }

           //QDataStream in(&file);
           bool isFirstLine = true;
           while (!file.atEnd())
           {
                QString line = file.readLine();
                QStringList splittedStr =  line.split(';');
                qDebug() << splittedStr;

                if (isFirstLine)
                {
                    cntrArea = splittedStr[1].toDouble();
                    isFirstLine = false;
                } else
                {
                    sizeItem currItem;
                    currItem.sizeID = splittedStr[0];
                    currItem.sizeInMilim = splittedStr[5].toInt();
                    currItem.admissibleErrorInMilim = splittedStr[6].toInt();

                    sizes.append(currItem);

                }
           }


           file.close();
           ui->pb_calibration->setEnabled(true);
    }
}

void QItemController::on_pb_calibration_clicked()
{
    timer->stop();
    emit calibrateCamera(currentFrame);
    emit saveImg(currentFrame);
}

void QItemController::setCalibParams(calibrationParameters _calibrParams)
{
     calibrParams = _calibrParams;
     if ((calibrParams.bRect.area()>0) && (calibrParams.pixelSizeInMilims > 0.0))
     {
        ui->pb_captureControl->setEnabled(true);
     }
}

void QItemController::setFrame(Mat &frame)
{
    ui->lb_itemImg->setFrame(frame);
    frame.copyTo(currentFrame);
    ui->pb_calibration->setEnabled(true);
}

void QItemController::setFrameNormalized(Mat &frame)
{
    ui->lb_imgNormalized->setFrame(frame);
    /*frame.copyTo(currentFrame);
    ui->pb_calibration->setEnabled(true);*/
}

void QItemController::on_pb_captureControl_clicked()
{
    /*QString fileName = QFileDialog::getOpenFileName(this,
           tr("Open Inpit Image"), "",
           tr("Item models (*.jpeg);;All Files (*)"));

    currentFrame = imread(fileName.toStdString());*/

    if ((calibrParams.bRect.area()>0) && (calibrParams.pixelSizeInMilims > 0.0))
    {

        Mat croppedFrame = currentFrame(calibrParams.bRect);



        std::vector<Vec4i> hierarchy;
        std::vector<std::vector<cv::Point>> contours;
        int largestContourID;


        imgUtils->performItemExtraction(croppedFrame, contours, hierarchy, largestContourID);

        Scalar color( 255, 255, 255);
        drawContours(croppedFrame, contours, largestContourID, color, /*CV_FILLED*/3, 8, hierarchy );

        ui->lb_itemImg->setFrame(croppedFrame);

        double area = contourArea(contours[largestContourID])/1000.0;


        Mat imgBlack = Mat::zeros(croppedFrame.rows, croppedFrame.cols,  croppedFrame.type());
        int numberOfPoints = (int)contours[largestContourID].size();
        const Point* elementPoints[1] = { &contours[largestContourID][0] };
        cv::fillPoly(imgBlack,elementPoints, &numberOfPoints, 1, Scalar(255, 255, 255));

        bitwise_and(croppedFrame, imgBlack, imgBlack);

        //get orientation
        double orientation = imgUtils->getOrientation(contours[largestContourID], imgBlack);

        //imshow("ORIENTATION", imgBlack);
        Moments mu;

        qDebug() << "Orientation: " << orientation;

        while (abs(orientation) > MAX_ERROR_RADIANS)
        {
            // Get the moment:
            mu = moments( contours[largestContourID], false );

            // Get the mass center:
            cv::Point2f mc = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );

            double degrees = orientation * (180.0/3.141592653589793238463);
            imgUtils->rotateImage(imgBlack, degrees, mc);

            //get orientation
            imgUtils->performItemExtraction(imgBlack, contours, hierarchy, largestContourID);
            orientation = imgUtils->getOrientation(contours[largestContourID], imgBlack);
            qDebug() << "Orientation: " << orientation;
        }


        area = contourArea(contours[largestContourID])/1000.0;

        qDebug() << "Area: " << area;

        Scalar color2(255, 255, 155);
        drawContours(imgBlack, contours, largestContourID, color2, /*CV_FILLED*/3, 8, hierarchy );

        setFrameNormalized(imgBlack);

        //imshow("FINAL", imgBlack);

        //append size item
        bool areaPassed = true;
        ui->tabWidget->setRowCount(0);
        ui->tabWidget->insertRow(ui->tabWidget->rowCount());
        QTableWidgetItem *itemID = new QTableWidgetItem("totalArea");
        if (abs(area - cntrArea) < 20)
        {
            itemID->setCheckState(Qt::Checked);
            ui->lbl_status->setText("PASSED");
            ui->lbl_status->setStyleSheet("QLabel {color : green; }");
        } else
        {
           itemID->setCheckState(Qt::Unchecked);
           ui->lbl_status->setText("FAILED");
           ui->lbl_status->setStyleSheet("QLabel {color : red; }");
           areaPassed = false;
        }
        ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 0, itemID);

        QTableWidgetItem *itemArea = new QTableWidgetItem(tr("%1").arg(area));
        ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 1, itemArea);

        QTableWidgetItem *itemAreaCntrl = new QTableWidgetItem(tr("%1").arg(cntrArea));
        ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 2, itemAreaCntrl);


        for (int i = 0; i < sizes.length(); i++)
        {
            ui->tabWidget->insertRow(ui->tabWidget->rowCount());
            QTableWidgetItem *itemID = new QTableWidgetItem(sizes[i].sizeID);
            if (areaPassed) itemID->setCheckState(Qt::Checked);
            else itemID->setCheckState(Qt::Unchecked);
            ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 0, itemID);

            QTableWidgetItem *itemSize = new QTableWidgetItem(tr("%1").arg(sizes[i].sizeInMilim));
            ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 1, itemSize);

            QTableWidgetItem *itemError = new QTableWidgetItem(tr("%1").arg(sizes[i].admissibleErrorInMilim));
            ui->tabWidget->setItem(ui->tabWidget->rowCount()-1, 2, itemError);
        }
    }

}

void QItemController::getFrame()
{
    emit requestFrame();
}

void QItemController::on_pushButton_clicked()
{
    timer->start(1.0/(float)FPS);
}
