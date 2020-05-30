#include "aq_basic_para.h"

basic_para::basic_para()
{
     M = 5;//mass of little car.
     m = 10;//mass of weight.
     l = 0.3;//length of cable.
     g = 9.81;// gravity acc.
     B1 = 0;//damp of littel car.
     B2 = 0;//damp of cable rotate.
     a_max = 1;//max acc of little car.M/S^2
     v_max = 0.5;//M/S
}

basic_para::~basic_para()
{

}

float basic_para::get_a_max(void)
{
    return a_max;
}


float basic_para::get_v_max(void)
{
    return a_max;
}

float basic_para::get_l(void)
{
    return l;
}


float basic_para::get_g(void)
{
    return g;
}

float basic_para::get_B1(void)
{
    return B1;
}


float basic_para::get_B2(void)
{
    return B2;
}

float basic_para::get_m(void)
{
    return m;

}

float basic_para::get_M(void)
{
    return M;
}

