#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QDateTime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "qsizeanalyzergui.h"
#include "ui_qsizeanalyzergui.h"

using namespace cv;

QSizeAnalyzerGUI::QSizeAnalyzerGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSizeAnalyzerGUI)
{
    ui->setupUi(this);
    cameraCtrl = new QRaspiCamControl();
    imgUtils = new QImageProcessingControl();

    //timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(getFrame()));
}

QSizeAnalyzerGUI::~QSizeAnalyzerGUI()
{
    delete ui;
    delete cameraCtrl;
    delete imgUtils;
}

void QSizeAnalyzerGUI::on_pb_addItem_clicked()
{
    dlgAddItem = new QAddItem(this);
    //qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(dlgAddItem, SIGNAL(requestFrame()), this, SLOT(setFrameToAddItemDlg()));
    connect(dlgAddItem, SIGNAL(calibrateCamera(cv::Mat&)), this, SLOT(performCalibration(cv::Mat&)));
    connect(dlgAddItem, SIGNAL(saveImg(cv::Mat&)), this, SLOT(saveImage(cv::Mat&)));
    //connect(dlgAddItem, SIGNAL(extractItemResend(cv::Mat&)), this, SLOT(extractItemSlot(cv::Mat&)));

    dlgAddItem->show();
}

void QSizeAnalyzerGUI::on_pb_control_clicked()
{
    dlgItemControl = new QItemController(this);
    //qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(dlgItemControl, SIGNAL(requestFrame()), this, SLOT(setFrameToControlDlg()));
    connect(dlgItemControl, SIGNAL(saveImg(cv::Mat&)), this, SLOT(saveImage(cv::Mat&)));
    //connect(dlgAddItem, SIGNAL(extractItemResend(cv::Mat&)), this, SLOT(extractItemSlot(cv::Mat&)));
    connect(dlgItemControl, SIGNAL(calibrateCamera(cv::Mat&)), this, SLOT(performCalibration(cv::Mat&)));
    dlgItemControl->show();
}

void QSizeAnalyzerGUI::setFrameToAddItemDlg()
{
    if (dlgAddItem != NULL)
    {
        Mat frame = cameraCtrl->getFrame();
        dlgAddItem->setFrame(frame);
    }

}

void QSizeAnalyzerGUI::performCalibration(Mat &frame)
{
    bool res = imgUtils->calibrateCamera(frame);
    if (res)
    {
        if (dlgAddItem != NULL) dlgAddItem->setCalibParams(imgUtils->getCalibrParams());
        if (dlgItemControl != NULL) dlgItemControl->setCalibParams(imgUtils->getCalibrParams());
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("The calibration was performed successfully");
        msgBox.exec();
    } else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Error occurred during calibration procedure");
        msgBox.exec();
    }

}

void QSizeAnalyzerGUI::extractItemSlot(Mat &frame)
{
    //imgUtils->performItemExtraction(frame);
}

void QSizeAnalyzerGUI::setFrameToControlDlg()
{
    if (dlgItemControl != NULL)
    {
        Mat frame = cameraCtrl->getFrame();
        dlgItemControl->setFrame(frame);
    }
}

void QSizeAnalyzerGUI::saveImage(Mat &img)
{
    QDir dir(".");
    QString path = dir.absolutePath() + "/images/";
    QDir dir_(path);
    if (!dir_.exists())
        dir_.mkpath(".");

    path+=  QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".jpeg";
    imwrite(path.toStdString(), img);
    qDebug() << "saveImage<<<" << path;
}

