#include "aq_basic_para.h"

//float M = 5;//mass of little car.
//float m = 10;//mass of weight.
//float l = 0.3;//length of cable.
//float g = 9.81;// gravity acc.
//float B1 = 0;//damp of littel car.
//float B2 = 0;//damp of cable rotate.
//float a_max = 2;//max acc of little car.
//float v_max = 1.5;

basic_para::basic_para()
{
     M = 5;//mass of little car.
     m = 10;//mass of weight.
     l = 0.3;//length of cable.
     g = 9.81;// gravity acc.
     B1 = 0;//damp of littel car.
     B2 = 0;//damp of cable rotate.
     a_max = 2;//max acc of little car.M/S^2
     v_max = 1.5;//M/S
}

basic_para::~basic_para()
{


}

void basic_para::set_Xd(const float Xd)
{
    Xd_ = Xd;
}


float basic_para::get_Xd(void)
{
    return Xd_;

}

void basic_para::set_Xc(const float Xc)
{
    Xc_ = Xc;
}


float basic_para::get_Xc(void)
{
    return Xc_;

}

void basic_para::set_Td(const float Td)
{
    Td_ = Td;
}

float basic_para::get_Td(void)
{
    return Td_;
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

