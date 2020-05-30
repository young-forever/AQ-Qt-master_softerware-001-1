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
#include "circle_queue.h"
#include "image_save_thread.h"

#define CAMERA_INDEX 0

using namespace std;

//extern unsigned char *pDataForSaveImage;

// state data struct:
typedef struct state_data
{
    float x_data;
    float dx_data;
    float theta_data;
    float dtheta_data;
    unsigned char* image_data;//iamge
}state_data;


class CameraWorkThread: public QThread
{
    Q_OBJECT

    bool save_enable_flag;
    string save_dir;
    int image_path;

    unsigned char*  image_buffer[100]; //store image buffer.

    Circle_Queue<unsigned char*>* m_queue_camera;

    image_save_thread* m_image_save_thread;

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

    void open_iamge_save(void);
    void close_image_save(void);
    void set_save_dir(string dir);

Q_SIGNALS:
    void show_image(unsigned char *pImage,unsigned short n);
    void refresh_camera_cnt_signal(int camera_cnt);
    void image_path_save(int path_);
};

#endif // CAMERAWORKTHREAD_H
