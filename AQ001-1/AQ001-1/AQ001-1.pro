#-------------------------------------------------
#
# Project created by QtCreator 2020-03-20T14:40:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AQ001-1
TEMPLATE = app
INCLUDEPATH += ./include
INCLUDEPATH += ./lib

SOURCES += main.cpp \
    mainwindow.cpp \
    my_serialport.cpp \
    data_analysis.cpp \
    circle_queue.cpp \
    controlthread.cpp \
    aq_planner.cpp \
    aq_controller.cpp \
    aq_basic_para.cpp \
    cameraworkthread.cpp \
    image_save_thread.cpp \
    sample_label.cpp

HEADERS  += mainwindow.h \
    my_serialport.h \
    data_analysis.h \
    circle_queue.h \
    controlthread.h \
    aq_planner.h \
    aq_controller.h \
    aq_basic_para.h \
    include/MvCameraControl.h \
    cameraworkthread.h \
    image_save_thread.h \
    sample_label.h

FORMS    += mainwindow.ui \
    data_analysis.ui

CONFIG += c++11


INCLUDEPATH += $$PWD/lib/64
DEPENDPATH += $$PWD/lib/64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../opt/MVS/lib/64/release/ -lMvCameraControl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../opt/MVS/lib/64/debug/ -lMvCameraControl
else:unix: LIBS += -L$$PWD/../../../../../../opt/MVS/lib/64/ -lMvCameraControl

INCLUDEPATH += $$PWD/../../../../../../opt/MVS/lib/64
DEPENDPATH += $$PWD/../../../../../../opt/MVS/lib/64
