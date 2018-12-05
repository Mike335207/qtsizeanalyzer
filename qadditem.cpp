#include "qadditem.h"
#include "ui_qadditem.h"

#include "utils.h"

using namespace cv;

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
}

void QAddItem::on_pb_getFrame_clicked()
{
    emit requestFrame();
}

void QAddItem::on_pb_OK_clicked()
{
    close();
}
