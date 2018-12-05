#ifndef QDISPLAYLABEL_H
#define QDISPLAYLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>

class QDisplayLabel: public QLabel
{
    Q_OBJECT

public:
    QDisplayLabel(QWidget *parent = nullptr);
    void setFrame(cv::Mat frame, int CROP_W = 0, int CROP_H = 0);
    bool isRequestClickPosition(bool isReq);

signals:
    void mouseClickPosition(QPoint);

protected:
    void mousePressEvent(QMouseEvent * ev);

private:
    bool isSendMouseClickPos = false;

};

#endif // QDISPLAYLABEL_H
