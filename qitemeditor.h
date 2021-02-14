#ifndef QITEMEDITOR_H
#define QITEMEDITOR_H

#include <QDialog>

#include "opencv2/opencv.hpp"

#include "struct.h"
#include "qimageprocessingcontrol.h"

namespace Ui {
class QItemEditor;
}

class QItemEditor : public QDialog
{
    Q_OBJECT

public:
    explicit QItemEditor(cv::Mat currentFrame, calibrationParameters params,  QWidget *parent = nullptr);
    ~QItemEditor();

public slots:
    void testPoint(QPoint pnt);
    void drawArrow(QPoint pnt);

signals:
    void extractItem(cv::Mat& frame);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::QItemEditor *ui;
    QImageProcessingControl* imgUtils = NULL;
    cv::Mat croppedFrame;
    std::vector<std::vector<cv::Point>> contours;
    int largestContourID;

    QList<cv::Point> usersPnts;
    QList<sizeItem> sizes;
    calibrationParameters calibParams;
    cv::Point2f mc;
    double cArea = 0;

    //utils
    void drawSizes();
};

#endif // QITEMEDITOR_H
