#ifndef QITEMCONTROLLER_H
#define QITEMCONTROLLER_H

#include <QDialog>
#include <QTimer>

#include "opencv2/core.hpp"
#include "struct.h"
#include "qimageprocessingcontrol.h"

namespace Ui {
class QItemController;
}

class QItemController : public QDialog
{
    Q_OBJECT

public:
    explicit QItemController(QWidget *parent = nullptr);
    ~QItemController();
    void setCalibParams(calibrationParameters _calibrParams);

    void setFrame(cv::Mat& frame);
    void setFrameNormalized(cv::Mat &frame);
signals:
    void calibrateCamera(cv::Mat& currentFrame);
    void requestFrame();
    void saveImg(cv::Mat &currentFrame);

private slots:
    void on_pb_loadModel_clicked();

    void on_pb_calibration_clicked();


    void on_pb_captureControl_clicked();

    void getFrame();

    void on_pushButton_clicked();

private:
    Ui::QItemController *ui;
    cv::Mat currentFrame;
    calibrationParameters calibrParams;
    QImageProcessingControl *imgUtils;

    QList<sizeItem> sizes;
    double cntrArea = 0;

    QTimer* timer;
};

#endif // QITEMCONTROLLER_H
