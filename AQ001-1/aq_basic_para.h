#ifndef AQ_BASIC_PARA_H
#define AQ_BASIC_PARA_H
#include "aq_basic_para.h"
#define PI 3.1415926

//extern float M;
//extern float m;
//extern float l;
//extern float g;
//extern float B1;
//extern float B2;

//extern float a_max;
//extern float v_max;

class basic_para
{
    float M;//mass of little car.
    float m;//mass of weight.
    float l;//length of cable.
    float g;// gravity acc.
    float B1;//damp of littel car.
    float B2;//damp of cable rotate.
    float a_max;//max acc of little car.
    float v_max;

    float Xd_;//desire position.
    float Xc_;// current positon;
    float Td_; //expect planner time.

    short PlannType;

public:
    basic_para();
    ~basic_para();
    void set_Xd(const float Xd);
    float get_Xd(void);
    void set_Xc(const float Xc);
    float get_Xc(void);
    void set_Td(const float Td);
    float get_Td(void);

    float get_a_max(void);
    float get_v_max(void);
    float get_l(void);
    float get_g(void);
    float get_B1(void);
    float get_B2(void);
};

#endif // AQ_BASIC_PARA_H
