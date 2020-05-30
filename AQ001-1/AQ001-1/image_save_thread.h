#ifndef IMAGE_SAVE_THREAD_H
#define IMAGE_SAVE_THREAD_H
#include <QThread>
#include <QMutex>
#include "circle_queue.h"
#include <QObject>
#include <qimage.h>

class image_save_thread: public QThread
{
    Q_OBJECT

    QImage* img;
    std::__cxx11::string save_dir;
//    int image_cnt;
//    Circle_Queue<state_data*>* m_state_data;
    Circle_Queue<unsigned char*>* m_queue_camera;


public slots:
    void stopImmediately();

private:
    //    my_serialport* my_serial_;
    QMutex m_lock;
    bool m_isCanRun;


public:
    image_save_thread(QObject *par,Circle_Queue<unsigned char*>* m_queue);
    ~image_save_thread();

    void set_save_dir(std::__cxx11::string dir);
    void run();


Q_SIGNALS:

};

#endif // IMAGE_SAVE_THREAD_H
