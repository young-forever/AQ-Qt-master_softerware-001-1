#ifndef AQ_PLANNER_H
#define AQ_PLANNER_H
#include "aq_basic_para.h"


typedef struct planner_para
{
    float Xd;
    float Xc;
    float Ld;
    float Lc;
    float Td;
}planner_para;

class AQ_planner
{
    float acc;
    float t_a;
    float t_total;

    float Xd_;
    float Xc_;

    float Vx_;
    float Td_;

    float a_max;
    float v_max;

    float a_mv;
    float v_mv;
    float t_acc;
    float t_con;
    float t_dacc;

    float delat_T;

    unsigned char planer_type;

    basic_para* m_basic_para;
    unsigned char controlmode;

public:
    AQ_planner();
    ~AQ_planner();

    unsigned char get_controlmode(void);

    unsigned char planner(basic_para *b_para, float xd, float xc, float td, const unsigned char plantype);
    float planner_update(const float t);

    void set_delat_T(const float T);

    //planner list.
    void planner_p(float x_d,float x_c,float t_d);
    float planner_update_p(const float t_c);

    void planner_step(float t_d);//unit: m;
    float planner_update_step(const float t_c);

    void planner_v(float x_d,float x_c,float t_d);
    float planner_update_v(const float t_c);

    //ZV PLANNER
    float planner_ZV(const float t);
    float planner_ZVD(const float t);
    float planner_ZVDD(const float t);

};

#endif // AQ_PLANNER_H
