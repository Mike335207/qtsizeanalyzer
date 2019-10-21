#ifndef QIMAGEPROCESSINGCONTROL_H
#define QIMAGEPROCESSINGCONTROL_H

#include <QObject>

class QImageProcessingControl : public QObject
{
    Q_OBJECT
public:
    explicit QImageProcessingControl(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QIMAGEPROCESSINGCONTROL_H