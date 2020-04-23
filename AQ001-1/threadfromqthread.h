#ifndef THREADFROMQTHREAD_H
#define THREADFROMQTHREAD_H

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
#include "my_serialport.h"

extern char data_test[8];

class ThreadFromQThread : public QThread
{
    Q_OBJECT

public slots:
    void stopImmediately();

private:
    my_serialport* my_serial_;
    QMutex m_lock;
    bool m_isCanRun;

public:
   ThreadFromQThread(QObject* par,my_serialport* serialport);
   ~ThreadFromQThread();

   void run();

Q_SIGNALS:
   void serial_ok_signal(void);
};


#endif //THREADFROMQTHREAD_H
