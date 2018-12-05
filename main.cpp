#include "qsizeanalyzergui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSizeAnalyzerGUI w;
    w.show();

    return a.exec();
}
