#ifndef CAMERAWORKTHREAD_H
#define CAMERAWORKTHREAD_H
#include <QThread>
#include <QMutex>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <iomanip>

#define CAMERA_INDEX 0

extern unsigned char *pDataForSaveImage;
extern bool CameraWorkFlag;

class CameraWorkThread: public QThread
{
    Q_OBJECT

    int image_cnt;
public slots:
    void stopImmediately();

private:
    //    my_serialport* my_serial_;
    QMutex m_lock;
    bool m_isCanRun;

public:
    CameraWorkThread(QObject *par);
    ~CameraWorkThread();
    void run();

    int get_image_cnt(void);

Q_SIGNALS:
    void show_image();

};

#endif // CAMERAWORKTHREAD_H
