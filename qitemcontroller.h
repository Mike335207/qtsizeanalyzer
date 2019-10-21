#ifndef QITEMCONTROLLER_H
#define QITEMCONTROLLER_H

#include <QDialog>

namespace Ui {
class QItemController;
}

class QItemController : public QDialog
{
    Q_OBJECT

public:
    explicit QItemController(QWidget *parent = nullptr);
    ~QItemController();

private:
    Ui::QItemController *ui;
};

#endif // QITEMCONTROLLER_H
