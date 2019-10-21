#include "qitemcontroller.h"
#include "ui_qitemcontroller.h"

QItemController::QItemController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QItemController)
{
    ui->setupUi(this);
}

QItemController::~QItemController()
{
    delete ui;
}
