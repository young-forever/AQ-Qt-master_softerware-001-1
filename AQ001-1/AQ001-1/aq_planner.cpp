#include "aq_planner.h"
#include "math.h"
#include <iostream>
#include "mainwindow.h"
using namespace std;

AQ_planner::AQ_planner()
{
    delat_T = 0.6;
}

AQ_planner::~AQ_planner()
{

}

unsigned char AQ_planner::get_controlmode()
{
    return controlmode;
}

unsigned char AQ_planner::planner(basic_para* b_para,float xd, float xc, float td,const unsigned char plantype)
{
    m_basic_para = b_para;
    a_max = b_para->get_a_max();
    v_max = b_para->get_v_max();

    Xd_ = xd;
    Xc_ = xc;
    Td_ = td;
    planer_type = plantype;

    //****** choose planner.*********//
    switch(plantype)
    {
    case STEP_PLANNER:
        planner_step(Td_);
        controlmode = VelocityMode;
        break;

    case S_POSITION_PLANNER:
        planner_p(Xd_, Xc_ ,Td_);
        controlmode = VelocityMode;
        break;

    case NANKAI_PLANNER:
        planner_step(Td_);
        controlmode = CurrentMode;
        break;

    case T_VELOCITY_PLANNER:
        planner_v(Xd_, Xc_ ,Td_);
        controlmode = VelocityMode;
        break;
    case STEP_VELOCITY_PLANNER:
        controlmode = VelocityMode;
        break;

    }

    return controlmode;
}

float AQ_planner::planner_update(const float t)
{
    float target;

    //********** choose update planner; *******//

    switch(planer_type)
    {
    case STEP_PLANNER:
        target = planner_update_step(t);
        break;

    case S_POSITION_PLANNER:
        target = planner_update_p(t);
        break;
    case NANKAI_PLANNER:
        target = planner_update_step(t);
        break;
    case T_VELOCITY_PLANNER:
        target = planner_update_v(t);
        break;

    case STEP_VELOCITY_PLANNER:
        target = Xd_;
        break;
    }

    return target;
}

// position planner
void AQ_planner::planner_p(float x_d, float x_c, float t_d)//unit: m;
{
    float distance = fabs(x_d - x_c);
    float T_max = 2*v_max/a_max;
    float T_c = 2*sqrt(distance/a_max);

    Xd_ = x_d;
    Xc_ = x_c;


    if(T_c<=T_max && T_c>=t_d)
    {
        t_acc = 0.5*T_c;
        t_con = 0.5*T_c;
        t_dacc = T_c;
    }
    else if(T_c >T_max)
    {
        v_mv = v_max;
        t_acc = v_max/a_max;
        t_dacc = (distance-a_max*t_acc*t_acc)/v_max+2*t_acc;
        t_con = t_dacc - t_acc;
    }
    else
    {
        float l_max = 1.0/4.0*a_max*t_d*t_d;
        float t1 = 0.5*t_d - sqrt((l_max - distance)/a_max);
        float v_cal_max = t1*a_max;

        if(v_cal_max <= v_max)//normal.
        {
            v_mv = v_cal_max;
            t_acc = t1;
            t_con = t1 + 2*sqrt((l_max - distance)/a_max);
            t_dacc = t_d;
        }
    }

    //judge direction.
    if(x_d - x_c>0)
    {
        v_mv = v_mv;
        a_mv = a_max;
    }
    else
    {
        v_mv = -v_mv;
        a_mv = -a_max;
    }
}

float AQ_planner::planner_update_p(const float t_c)
{
    float desire_p;

    if(t_c<0)
    {
        return Xc_;

    }

    if(t_c<=t_acc)
    {
        desire_p = 0.5*a_mv*t_c*t_c;
    }
    else if(t_c<=t_con)
    {
        desire_p = 0.5*a_mv*t_acc*t_acc + (t_c - t_acc)*v_mv;
    }
    else if(t_c<=t_dacc)
    {
        desire_p = a_mv*t_acc*t_acc + (t_con - t_acc)*v_mv - 0.5*a_mv*(t_dacc-t_c)*(t_dacc-t_c);
    }
    else
    {
        desire_p = a_mv*t_acc*t_acc + (t_con - t_acc)*v_mv;
    }
    return desire_p+Xc_;
}

// velocity planner
void AQ_planner::planner_v(float x_d, float x_c, float t_d)//unit: m;
{
    planner_p(x_d, x_c, t_d);
}

float AQ_planner::planner_update_v(const float t_c)
{
    float desire_v;

    if(t_c<0)
    {
        return 0;
    }

    if(t_c<=t_acc)
    {
        desire_v = a_mv*t_c;
    }
    else if(t_c<=t_con)
    {
        desire_v = v_mv;
    }
    else if(t_c<=t_dacc)
    {
        desire_v = v_mv - a_mv*(t_c-t_con);
    }
    else
    {
        desire_v = 0;
    }

//    cout<<"v_mv: "<<v_mv<<" "<<"a_mv: "<<a_mv<<"  "<<"t_con: "<<t_con<<"  "<<"t_c: "<<t_c<<"  "<<endl;
    return desire_v;
}

// step planner
void AQ_planner::planner_step(float t_d)//unit: m;
{
    t_acc = t_d;
}

float AQ_planner::planner_update_step(const float t_c)
{
    if(t_c<=0)
    {
        return Xc_;
    }

    return Xd_;
}

void AQ_planner::set_delat_T(const float T)
{
    delat_T = T;
}


float AQ_planner::planner_ZV(const float t)
{
//    float B1_ = m_basic_para->get_B1();
//    float B2_ = m_basic_para->get_B2();

//    float K = pow(2.71828,-B2_*PI/sqrt(1-B1_*B1_));

    float d_T =delat_T;

    return 0.5*planner_update(t)+0.5*planner_update(t-d_T);
}

float AQ_planner::planner_ZVD(const float t)
{
//    float B1_ = m_basic_para->get_B1();
//    float B2_ = m_basic_para->get_B2();

//    float K = pow(2.71828,-B2_*PI/sqrt(1-B1_*B1_));

    float d_T =delat_T;

    return 0.25*planner_update(t)+0.5*planner_update(t-d_T)+0.25*planner_update(t-2*d_T);
}

float AQ_planner::planner_ZVDD(const float t)
{
//    float B1_ = m_basic_para->get_B1();
//    float B2_ = m_basic_para->get_B2();

//    float K = pow(2.71828,-B2_*PI/sqrt(1-B1_*B1_));

    float d_T =delat_T;

    return 0.125*planner_update(t)+0.375*planner_update(t-d_T)+0.375*planner_update(t-2*d_T)+0.125*planner_update(t-3*d_T);
}
