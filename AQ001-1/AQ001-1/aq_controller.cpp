#include "aq_controller.h"
#include "mainwindow.h"

using namespace std;

float Kp = 0;
float Ki = 0;
float Kd = 0;

AQ_controller::AQ_controller()
{
    controller_reset();

    control_out = 0;

    m_basic_para = new basic_para();
}

AQ_controller::~AQ_controller(void)
{

}

float AQ_controller::controller_reset(void)
{
    Xd_ = 0;
    d_q_last = 0;
    d_q = 0;
    dd_q = 0;
    i_item = 0;
}

float AQ_controller::controller(float Xd)
{
    Xd_ = Xd;
    d_q_last = d_q;
    d_q = Xd_ - position_x;
    dd_q = d_q - d_q_last;
    i_item = i_item + d_q;

    control_out = Kp*d_q+Ki*i_item+Kd*dd_q;
}


float AQ_controller::controller_nankai()
{
    float m = m_basic_para->get_m();
    float l = m_basic_para->get_l();
    float g = m_basic_para->get_g();

    float i_theta = theta/180*3.1415926;
    float i_dtheta =  d_theta/180*3.1415926;

    float n_t = m*l*sin(i_theta)*i_dtheta*i_dtheta+m*(g+i_dtheta*d_position_x/1000.0)*sin(i_theta)*cos(i_theta);
    float y = (Ki*i_theta*i_theta+1)*d_position_x/1000.0;

    float e = position_x/1000.0 - Xd_/1000.0;

    control_out = -Kp*e-Kd*y - n_t;
}

float AQ_controller::get_control_out(void)
{
    return control_out;
}


void AQ_controller::set_Xd(float Xd)
{
    Xd_ = Xd;
}
