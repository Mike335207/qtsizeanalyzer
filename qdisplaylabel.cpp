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
    cv::resize(frame, dest, Size(frame.cols/2, frame.rows/2));
    cvtColor(dest, dest,CV_BGR2RGB);

    QImage image(dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);

    QPixmap pixmap = QPixmap::fromImage(image);

    int w = this->width();
    int h = this->height();

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    this->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));

}

bool QDisplayLabel::isRequestClickPosition(bool isReq)
{
    isSendMouseClickPos = isReq;
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
