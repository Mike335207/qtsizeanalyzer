#ifndef QADDITEM_H
#define QADDITEM_H

#include <opencv2/opencv.hpp>

#include <QDialog>
#include <QTimer>

#include "struct.h"
#include "qitemeditor.h"

namespace Ui {
class QAddItem;
}

class QAddItem : public QDialog
{
    Q_OBJECT

public:
    explicit QAddItem(QWidget *parent = 0);
    ~QAddItem();

    void setFrame(cv::Mat& frame);
    void setCalibParams(calibrationParameters _calibrParams);

signals:
    void requestFrame();
    void calibrateCamera(cv::Mat &currentFrame);
    void extractItemResend(cv::Mat&);
    void saveImg(cv::Mat&);

private slots:
    void on_pb_getFrame_clicked();

    void on_pb_OK_clicked();

    void on_pb_calibrate_clicked();

    void on_pb_editItem_clicked();

    void extractItemSlot(cv::Mat& img);

    void getFrame();

private:
    Ui::QAddItem *ui;
    cv::Mat currentFrame;
    calibrationParameters calibrParams;

    QItemEditor* dlgEditor = NULL;
    QTimer* timer = NULL;

    int frameCounter = 0;
};

#endif // QADDITEM_H
