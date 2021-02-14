#include "qdisplaylabel.h"

using namespace cv;

#include <QImage>
#include <QPixmap>
#include <QDebug>


QDisplayLabel::QDisplayLabel(QWidget *parent) :
    QLabel(parent)
{
}


void QDisplayLabel::setFrame(cv::Mat frame, int CROP_W, int CROP_H)
{
    Mat dest;
    /*cv::resize(frame, dest, Size(frame.cols/2, frame.rows/2));*/
    cvtColor(frame, dest,CV_BGR2RGB);

    QImage image(dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);

    QPixmap pixmap = QPixmap::fromImage(image);

    int w = this->width();
    int h = this->height();

    qDebug() << "w=" << w << "; h=" << h;

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    this->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
    this->setMouseTracking(true);

}

bool QDisplayLabel::isRequestClickPosition(bool isReq)
{
    isSendMouseClickPos = isReq;
}

bool QDisplayLabel::isRequestMouseMovePosition(bool isReq)
{
    isSendMouseMovePos = isReq;
}

void QDisplayLabel::mousePressEvent(QMouseEvent *ev)
{
   QPoint pnt = ev->pos();
   if (isSendMouseClickPos)
   {
        qDebug() << "Mouse pos: [" << pnt.x() << "; " << pnt.y() << "]";
        emit mouseClickPosition(pnt);
   }
}

void QDisplayLabel::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint pnt = ev->pos();
    if (isSendMouseMovePos)
    {
         qDebug() << "Mouse pos: [" << pnt.x() << "; " << pnt.y() << "]";
         emit mouseMovePosition(pnt);
    }
}
