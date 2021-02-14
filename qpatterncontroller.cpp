#include "qpatterncontroller.h"
#include "ui_qpatterncontroller.h"

QPatternController::QPatternController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPatternController)
{
    ui->setupUi(this);
}

QPatternController::~QPatternController()
{
    delete ui;
}
