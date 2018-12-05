#include "qadditem.h"
#include "ui_qadditem.h"

#include "utils.h"

using namespace cv;

#define SCALING_F 0.5

QAddItem::QAddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QAddItem)
{
    ui->setupUi(this);
    show();
}

QAddItem::~QAddItem()
{
    delete ui;
}

void QAddItem::setFrame(cv::Mat &frame)
{
    ui->lb_liveStream->setFrame(frame);
    frame.copyTo(currentFrame);
}

void QAddItem::on_pb_getFrame_clicked()
{
    emit requestFrame();
}

void QAddItem::on_pb_OK_clicked()
{
    close();
}

void QAddItem::on_pb_calibrate_clicked()
{
    Mat tempFrame;
    if (!currentFrame.empty())
    {
        currentFrame.copyTo(tempFrame);

        cv::resize(tempFrame, tempFrame, Size(tempFrame.cols*SCALING_F, tempFrame.rows*SCALING_F));

        std::vector<std::vector<cv::Point> > squares;
        findSquares(tempFrame, squares);
        drawSquares(tempFrame, squares);
        ui->lb_liveStream->setFrame(tempFrame);
    }
}
