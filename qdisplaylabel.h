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
    bool isRequestMouseMovePosition(bool isReq);

signals:
    void mouseClickPosition(QPoint);
    void mouseMovePosition(QPoint);

protected:
    void mousePressEvent(QMouseEvent * ev);
    void mouseMoveEvent(QMouseEvent * ev);

private:
    bool isSendMouseClickPos = false;
    bool isSendMouseMovePos = false;

};

#endif // QDISPLAYLABEL_H
