#ifndef AQ_CONTROLLER_H
#define AQ_CONTROLLER_H
#include <QObject>
#include "aq_basic_para.h"

namespace Ui {
class MainWindow;
}

extern float Kp;
extern float Ki;
extern float Kd;


class AQ_controller
{
    float Xd_;//desire control value.

    //pid para:
    float d_q_last;
    float d_q;
    float dd_q;
    float i_item;

    float control_out;

    basic_para* m_basic_para;
public:
    AQ_controller();
    ~AQ_controller();

    float get_control_out(void); //return
    void set_Xd(float Xd);
    float controller_reset(void);

    //controller list:
    float controller(float Xd);//
    float controller_nankai();

};

#endif // AQ_CONTROLLER_H
