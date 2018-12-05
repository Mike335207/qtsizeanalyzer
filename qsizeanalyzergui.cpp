#include "qsizeanalyzergui.h"
#include "ui_qsizeanalyzergui.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

QSizeAnalyzerGUI::QSizeAnalyzerGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSizeAnalyzerGUI)
{
    ui->setupUi(this);
    cameraCtrl = new QRaspiCamControl();
}

QSizeAnalyzerGUI::~QSizeAnalyzerGUI()
{
    delete ui;
    delete cameraCtrl;
}

void QSizeAnalyzerGUI::on_pb_addItem_clicked()
{
    dlgAddItem = new QAddItem(this);

    connect(dlgAddItem, SIGNAL(requestFrame()), this, SLOT(setFrameToAddItemDlg()));

    dlgAddItem->show();
}

void QSizeAnalyzerGUI::on_pb_control_clicked()
{

}

void QSizeAnalyzerGUI::setFrameToAddItemDlg()
{
    if (dlgAddItem != NULL)
    {
        Mat frame = cameraCtrl->getFrame();
        dlgAddItem->setFrame(frame);
    }

}
