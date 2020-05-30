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
#include <QObject>
#include "aq_controller.h"
#include "aq_planner.h"
#include "my_serialport.h"


#define ZV_PLANN_TYPE       0X01
#define ZVD_PLANN_TYPE      0X02
#define ZVDD_PLANN_TYPE     0X03

namespace Ui {
class MainWindow;
}

class controlthread : public QThread
{
    Q_OBJECT

private:

    AQ_planner* m_AQ_planner[2];

    AQ_controller* m_controller;
    my_serialport* m_command_;

    basic_para* m_basic_para;
    planner_para* m_planner_para;

    unsigned char plantype_;
    float delat_T;

    float Xd_;
    float Td_;
    float target_vl;
    float target_vx;
    int timeout;
    bool mode_set_flag[2];
    bool drop_flag;
    unsigned char ZV_flag;
    bool noimage_sample_flag;

    QMutex m_lock;
    bool m_isCanRun;

public slots:
    void stopImmediately();
    void ZV_tpye_recieve(const unsigned char zv_type);
    void set_delat_T(const float T);
    void set_plantype(const unsigned char plantype);
    void set_noimage_sample_flag(void);
    void drop_flag_slot(bool flag);

public:
    controlthread(my_serialport* serial);
    ~controlthread();
    void send_control_mode(const short motor_num,const unsigned char control_mode);
    void send_control_value(const short motor_num, const unsigned char control_mode, const float control_value);

    void set_para(basic_para *b_para,planner_para *p_para );
    float get_target_vl(void);
    float get_target_vx(void);

    //    void set_Xd(const float Xd);
    //    void set_Td(const float t_d);
    void run();

Q_SIGNALS:
    void control_stop_signal(void);
    void noimage_aotu_sample(int path);
    void drop_weight_signal(void);

};

#endif // CONTROLTHREAD_H
