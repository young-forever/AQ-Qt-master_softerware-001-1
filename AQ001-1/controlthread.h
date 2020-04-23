#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H
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
#include "my_command.h"
#include <QObject>
#include "aq_controller.h"
#include "aq_planner.h"
namespace Ui {
class MainWindow;
}

class controlthread : public QThread
{
    Q_OBJECT

private:
    AQ_planner* m_AQ_planner;
    AQ_controller* m_controller;
    my_command* m_command_;

    basic_para* m_basic_para;

    float Xd_;
    float Td_;
    int timeout;

    QMutex m_lock;
    bool m_isCanRun;

public slots:
    void stopImmediately();

public:
    controlthread(my_command* m_command);
    ~controlthread();

    void set_para(basic_para *para);
//    void set_Xd(const float Xd);
//    void set_Td(const float t_d);
    void run();
};

#endif // CONTROLTHREAD_H
