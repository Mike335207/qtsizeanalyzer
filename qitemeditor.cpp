#include "qitemeditor.h"
#include "ui_qitemeditor.h"

QItemEditor::QItemEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QItemEditor)
{
    ui->setupUi(this);
}

QItemEditor::~QItemEditor()
{
    delete ui;
}
