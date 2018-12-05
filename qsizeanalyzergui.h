#ifndef QSIZEANALYZERGUI_H
#define QSIZEANALYZERGUI_H

#include <QWidget>

#include "qraspicamcontrol.h"
#include "qadditem.h"

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

private:
    Ui::QSizeAnalyzerGUI *ui;
    QRaspiCamControl* cameraCtrl = NULL;

    QAddItem* dlgAddItem = NULL;
};

#endif // QSIZEANALYZERGUI_H
