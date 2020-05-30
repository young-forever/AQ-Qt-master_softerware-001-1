#include "controlthread.h"
#include "mainwindow.h"

//#include <sys/time.h>
//struct timeval tpstart,tpend;
//double timeuse;

using namespace std;


controlthread::controlthread(my_serialport* serial) : m_command_(serial)
{
    m_controller = new AQ_controller();
    m_isCanRun = true;
    delat_T = 0;

    drop_flag = false;
    Xd_ = 0;
    Td_ = 0;

    timeout = 20*1000;//20000us.

    m_AQ_planner[0] = new AQ_planner();
    m_AQ_planner[1] = new AQ_planner();

    m_basic_para = new basic_para();
    m_planner_para = new planner_para();

    mode_set_flag[2] = {0};
    noimage_sample_flag = false;
}

controlthread::~controlthread()
{

}


void controlthread::send_control_mode(const short motor_num,const unsigned char control_mode)
{
    //choose control mode:
    switch(control_mode)
    {
    case VelocityMode:
        if(MotorMode[motor_num-1] != VelocityMode){
            MotorMode[motor_num-1] = VelocityMode;
            m_command_->send_motor_mode_choose(motor_num,VelocityMode);
            usleep(2000);
        }
        break;
    case CurrentMode:
        if(MotorMode[motor_num-1] != CurrentMode){
            MotorMode[motor_num-1] = CurrentMode;
            m_command_->send_motor_mode_choose(motor_num,CurrentMode);
            usleep(2000);
        }
        break;
    case PositionMode:
        if(MotorMode[motor_num-1] != PositionMode){
            MotorMode[motor_num-1] = PositionMode;
            m_command_->send_motor_mode_choose(motor_num,PositionMode);
            usleep(2000);
        }
        break;
    }

}

void controlthread::send_control_value(const short motor_num,const unsigned char control_mode, const float control_value)
{
    //send control value:
    switch(control_mode)
    {
    case VelocityMode:
        m_command_->send_velocity_send(motor_num,control_value);//rpm.
        break;
    case CurrentMode:
        m_command_->send_current_send(motor_num,control_value);//mA
        break;
    case PositionMode:
        m_command_->send_position_send(motor_num,control_value);//deg
        break;
    }
}


void controlthread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

void controlthread::ZV_tpye_recieve(const unsigned char zv_type)
{
    ZV_flag = zv_type;
}

void controlthread::set_delat_T(const float T)
{
    delat_T = T;

    m_AQ_planner[0]->set_delat_T(T);

}

void controlthread::set_plantype(const unsigned char plantype)
{
    plantype_ = plantype;
}

void controlthread::set_noimage_sample_flag(void)
{
    noimage_sample_flag = true;
}

void controlthread::drop_flag_slot(bool flag)
{
    drop_flag = flag;
}

// doing before run.
void controlthread::set_para(basic_para* b_para, planner_para *p_para)
{
    m_basic_para = b_para;
    m_planner_para = p_para;
}

float controlthread::get_target_vl(void)
{
    QMutexLocker locker(&m_lock);
    return target_vl;
}
float controlthread::get_target_vx(void)
{
    QMutexLocker locker(&m_lock);
    return target_vx;
}

void controlthread::run()
{
    m_isCanRun = true;
    mode_set_flag[0] = false;
    mode_set_flag[1] = false;
    unsigned char control_mode[2] = {0};

    int cnt = 0;
    short delay_cnt = 0;

    // choose ZV planner type:
    switch(ZV_flag){
    case ZV_PLANN_TYPE:
        delay_cnt = 1;
        break;
    case ZVD_PLANN_TYPE:
        delay_cnt = 2;
        break;
    case ZVDD_PLANN_TYPE:
        delay_cnt=3;
        break;
    default:
        delay_cnt = 0;
        break;
    }

    //calculate control paraments
    control_mode[0] = m_AQ_planner[0]->planner(m_basic_para,m_planner_para->Xd,m_planner_para->Xc,m_planner_para->Td,plantype_);//x planner caculate.
    control_mode[1] = m_AQ_planner[1]->planner(m_basic_para,m_planner_para->Ld,m_planner_para->Lc,m_planner_para->Td+delat_T*delay_cnt,T_VELOCITY_PLANNER);//l planner caculate.

    m_controller->controller_reset();

    send_control_mode(1,control_mode[0]);
    usleep(1500);
    send_control_mode(2,control_mode[0]);
    usleep(1500);
    m_command_->send_motor_run();
    usleep(1500);

    //control loop.
    do {
        usleep(timeout);

        /***** update time. ******/
        float t = cnt*timeout/1000000.0;

        /***** update planner.******/
        float target_x = m_AQ_planner[0]->planner_update(t)*1000.0;

        m_lock.lock();
        target_vl = m_AQ_planner[1]->planner_update(t)*1000.0;

        // choose ZV planner type:
        switch(ZV_flag){
        case ZV_PLANN_TYPE:
            target_x = m_AQ_planner[0]->planner_ZV(t)*1000.0;
            break;
        case ZVD_PLANN_TYPE:
            target_x = m_AQ_planner[0]->planner_ZVD(t)*1000.0;
            break;
        case ZVDD_PLANN_TYPE:
            target_x = m_AQ_planner[0]->planner_ZVDD(t)*1000.0;
            break;
        }

        float t_delay=0;
        /********* update controller.********/
        if(plantype_ == S_POSITION_PLANNER)
        {
            target_vx = m_controller->controller(target_x);//input control target and output control value.
            t_delay = 0.5;
        }
        else if(plantype_ == STEP_VELOCITY_PLANNER)
        {
            target_vx = target_x;
            t_delay = 0;
        }
        //m_controller->controller_nankai();

        /******* send command. **********/
        send_control_value(1,control_mode[0],target_vx*60/125);
        //        usleep(2000);
        //        send_control_value(2,control_mode[1],60.0*target_vl/(78.5));// m/s transform to rpm.
        //        cout<<"t: "<<t<<" "<<"target_vx: "<<target_vx<<"  "<<"target_vl: "<<target_vl<<"  "<<"delat_T:"<<delat_T<<"  "<<endl;

        if(noimage_sample_flag == true)
        {
            emit noimage_aotu_sample(0);
        }

        m_lock.unlock();
        if(t-delat_T*delay_cnt-t_delay>m_planner_para->Td)
        {
            stopImmediately();
            emit control_stop_signal();
            noimage_sample_flag = false;
        }

        QMutexLocker locker(&m_lock);
        if(!m_isCanRun)
        {
            usleep(20000);
            m_command_->send_motor_stop();//stop motor.
            cout<<"trage moving ok!"<<endl;
            usleep(500);
            if(drop_flag == true)
            {
                drop_flag = false;
                emit drop_weight_signal();
            }

            return;
        }
        cnt++;
    } while (1);
}
