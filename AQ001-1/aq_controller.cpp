#include "aq_controller.h"
#include "mainwindow.h"

using namespace std;

float Kp = 0;
float Ki = 0;
float Kd = 0;

AQ_controller::AQ_controller()
{
    d_q_last = 0;
    d_q = 0;
    dd_q = 0;
    i_item = 0;
    control_out = 0;

    Xd_ = 0;
}

AQ_controller::~AQ_controller(void)
{



}

float AQ_controller::controller()
{
    d_q_last = d_q;
    d_q = Xd_ - position_x;
    dd_q = d_q - d_q_last;
    i_item = i_item + d_q;

    control_out = Kp*d_q+Ki*i_item+Kd*dd_q;
//    control_out = Xd_;

//    cout<<"XD_: "<<Xd_<<"     "<<"X: "<<position_x<<endl;
}



float AQ_controller::get_control_out(void)
{
    return control_out;
}


void AQ_controller::set_Xd(float Xd)
{
    Xd_ = Xd;
}
