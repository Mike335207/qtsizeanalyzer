#ifndef QPATTERNCONTROLLER_H
#define QPATTERNCONTROLLER_H

#include <QDialog>

namespace Ui {
class QPatternController;
}

class QPatternController : public QDialog
{
    Q_OBJECT

public:
    explicit QPatternController(QWidget *parent = nullptr);
    ~QPatternController();

private:
    Ui::QPatternController *ui;
};

#endif // QPATTERNCONTROLLER_H
