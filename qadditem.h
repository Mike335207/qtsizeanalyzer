#ifndef QADDITEM_H
#define QADDITEM_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDialog>

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

signals:
    void requestFrame();

private slots:
    void on_pb_getFrame_clicked();

    void on_pb_OK_clicked();

    void on_pb_calibrate_clicked();

private:
    Ui::QAddItem *ui;
    cv::Mat currentFrame;
};

#endif // QADDITEM_H
