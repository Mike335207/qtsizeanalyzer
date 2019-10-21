#ifndef QITEMEDITOR_H
#define QITEMEDITOR_H

#include <QDialog>

namespace Ui {
class QItemEditor;
}

class QItemEditor : public QDialog
{
    Q_OBJECT

public:
    explicit QItemEditor(QWidget *parent = nullptr);
    ~QItemEditor();

private:
    Ui::QItemEditor *ui;
};

#endif // QITEMEDITOR_H
