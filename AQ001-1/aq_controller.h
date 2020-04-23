#ifndef AQ_CONTROLLER_H
#define AQ_CONTROLLER_H
#include <QObject>

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
public:
    AQ_controller();
    ~AQ_controller();

    float controller();//
    float get_control_out(void); //return
    void set_Xd(float Xd);
};

#endif // AQ_CONTROLLER_H
