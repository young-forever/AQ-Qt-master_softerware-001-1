#ifndef AQ_PLANNER_H
#define AQ_PLANNER_H
#include "aq_basic_para.h"

class AQ_planner
{
    float acc;
    float t_a;
    float t_total;

//    float desire_v;
//    float desire_p;

    float Xd_;
    float Xc_;
    float Td_;

    float a_max;
    float v_max;

    float a_mv;
    float v_mv;
    float t_acc;
    float t_con;
    float t_dacc;

    basic_para* m_basic_para;

public:
    AQ_planner();
    ~AQ_planner();

    void planner(basic_para *para);
    float planner_update(const float t);


    //planner list.
    void planner_p(float x_d,float x_c,float t_d);
    float planner_update_p(const float t_c);

    void planner_v(float x_d,float x_c);
    float planner_update_v(const float t_c);

    void planner_step(float t_d);//unit: m;
    float planner_update_step(const float t_c);

    //ZVD
    float planner_ZVD(const float t);

};

#endif // AQ_PLANNER_H
