#include "aq_planner.h"
#include "math.h"
#include <iostream>
#include "mainwindow.h"
using namespace std;

AQ_planner::AQ_planner()
{

}

AQ_planner::~AQ_planner()
{

}

void AQ_planner::planner(basic_para* para)
{
    m_basic_para = para;
    a_max = para->get_a_max();
    v_max = para->get_v_max();
    Xd_ = para->get_Xd();
    Xc_ = para->get_Xc();
    Td_ = para->get_Td();

    //****** choose planner.*********//
    switch(PlanType)
    {
    case STEP_PLANNER:
        planner_step(Td_);
        break;

    case S_POSITION_PLANNER:
        planner_p(Xd_, Xc_ ,Td_);
        break;
    }



    //planner_v(Xd_, Xc_);


}

float AQ_planner::planner_update(const float t)
{
    float target;

    //********** choose update planner; *******//

    switch(PlanType)
    {
        case STEP_PLANNER:
            target = planner_update_step(t);
        break;

        case S_POSITION_PLANNER:
            target = planner_update_p(t);
        break;
    }


    //target = planner_update_v(t);

    return target;
}


//velocity planner.
void AQ_planner::planner_v(float x_d, float x_c)//unit: m;
{
    float T_l = 2*PI*sqrt(m_basic_para->get_l() / m_basic_para->get_g());//calculate time of cable cycle.
    float l_amax = 1/4.0*a_max*T_l*T_l;

    float distance = fabs(x_d - x_c);

    if(l_amax <= distance)
    {
        t_acc = T_l;
        t_con = T_l + ((distance - l_amax)/a_max/t_acc);
        t_dacc = t_con+t_acc;
        a_mv = a_max;
        v_mv = t_acc*a_mv;
    }
    else
    {
        a_mv = distance/(T_l*T_l);
        t_acc = T_l;
        t_con = T_l;
        t_dacc = t_con+t_acc;
        v_mv = t_acc*a_mv;
    }

    //judge direction.
    if(x_d - x_c>0)
    {
    }
    else
    {
        v_mv = -v_mv;
        a_mv = -a_mv;
    }
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
        desire_v = a_mv*t_c;//acc stage.
    }
    else if(t_c<=t_con)
    {
        desire_v = v_mv;//uniform stage.
    }
    else if(t_c<=t_dacc)
    {
        desire_v = a_mv*(t_dacc-t_c);//reduce stage.
    }
    else
    {
        desire_v = 0;//stop stage.
    }
    return desire_v;
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
// step planner
void AQ_planner::planner_step(float t_d)//unit: m;
{
    t_acc = t_d;
}

float AQ_planner::planner_update_step(const float t_c)
{
    if(t_c<=t_acc)
    {
        return Xc_;
    }

    return Xd_;
}

float AQ_planner::planner_ZVD(const float t)
{
    float B1_ = m_basic_para->get_B1();
    float B2_ = m_basic_para->get_B2();

    //    float w = sqrt(g/l);
    //    float d_T = PI/(w*sqrt(1-B2*B2));
    float K = pow(2.71828,-B2_*PI/sqrt(1-B1_*B1_));

    float d_T =0.6;

    return 1/(K+2*K+K*K)*planner_update(t)+2*K/(K+2*K+K*K)*planner_update(t-d_T)+K*K/(K+2*K+K*K)*planner_update(t-2*d_T);

    //return 1/(K+2*K+K*K)*planner_update_v(t)+2*K/(K+2*K+K*K)*planner_update_v(t-d_T)+K*K/(K+2*K+K*K)*planner_update_v(t-2*d_T);
}
