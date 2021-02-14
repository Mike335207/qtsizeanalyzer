#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T20:49:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSizeAnalyzer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#RASPICAM
#unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lraspicam_cv

INCLUDEPATH += $$PWD/../../../../usr/local/include/raspicam
DEPENDPATH += $$PWD/../../../../usr/local/include/raspicam

#OPEN_CV

INCLUDEPATH += /usr/local/include/opencv4/

unix:!macx: LIBS += -lopencv_highgui
unix:!macx: LIBS += -lopencv_core
unix:!macx: LIBS += -lopencv_imgproc
unix:!macx: LIBS += -lopencv_imgcodecs
unix:!macx: LIBS += -lopencv_videoio
unix:!macx: LIBS += -lopencv_video



SOURCES += main.cpp\
        qsizeanalyzergui.cpp \
    qraspicamcontrol.cpp \
    qadditem.cpp \
    utils.cpp \
    qdisplaylabel.cpp \
    qimageprocessingcontrol.cpp \
    qitemeditor.cpp \
    qitemcontroller.cpp

HEADERS  += qsizeanalyzergui.h \
    qraspicamcontrol.h \
    qadditem.h \
    utils.h \
    qdisplaylabel.h \
    qimageprocessingcontrol.h \
    const.h \
    struct.h \
    qitemeditor.h \
    qitemcontroller.h

FORMS    += qsizeanalyzergui.ui \
    qadditem.ui \
    qitemeditor.ui \
    qitemcontroller.ui

RESOURCES +=
