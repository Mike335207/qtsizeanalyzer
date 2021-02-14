#ifndef QSIZEANALYZERGUI_H
#define QSIZEANALYZERGUI_H

#include <QWidget>

#include "qraspicamcontrol.h"
#include "qadditem.h"
#include "qimageprocessingcontrol.h"
#include "qitemcontroller.h"

namespace Ui {
class QSizeAnalyzerGUI;
}

class QSizeAnalyzerGUI : public QWidget
{
    Q_OBJECT

public:
    explicit QSizeAnalyzerGUI(QWidget *parent = 0);
    ~QSizeAnalyzerGUI();

private slots:
    void on_pb_addItem_clicked();

    void on_pb_control_clicked();


    void setFrameToAddItemDlg();
    void performCalibration(cv::Mat& frame);

    void extractItemSlot(cv::Mat& frame);

    void setFrameToControlDlg();

    void saveImage(cv::Mat& img);


private:
    Ui::QSizeAnalyzerGUI *ui;
    QRaspiCamControl* cameraCtrl = NULL;

    QAddItem* dlgAddItem = NULL;
    QImageProcessingControl* imgUtils = NULL;
    QItemController* dlgItemControl = NULL;
};

#endif // QSIZEANALYZERGUI_H
