#include "controlthread.h"
#include "mainwindow.h"

//#include <sys/time.h>
//struct timeval tpstart,tpend;
//double timeuse;

using namespace std;


controlthread::controlthread(my_command* m_command) : m_command_(m_command)
{
    m_controller = new AQ_controller();
    m_isCanRun = true;
    Xd_ = 0;
    Td_ = 0;

    timeout = 20*1000;//20000us.
    m_AQ_planner = new AQ_planner();
    m_basic_para = new basic_para();
}

controlthread::~controlthread()
{

}


void controlthread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}


// doing before run.
void controlthread::set_para(basic_para* para)
{
    m_basic_para = para;
}

void controlthread::run()
{
    m_isCanRun = true;

    int cnt = 0;
    //calculate control paraments
    m_AQ_planner->planner(m_basic_para);//planner caculate.

    //control loop.
    do {
//                gettimeofday(&tpend,NULL);
//                timeuse = (1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec)/1000000.0;
//                cout << "time use:"<<timeuse <<"s."<<endl;
//                gettimeofday(&tpstart,NULL);

        usleep(timeout);
        //update time.
        float t = cnt*timeout/1000000.0;

        //update planner.
        float target = m_AQ_planner->planner_update(t)*1000.0;

        if(ZVD_flag == true)
        {
            target = m_AQ_planner->planner_ZVD(t)*1000.0;
        }

        //update controller.
        m_controller->set_Xd(target);
        m_controller->controller();//get control value.
        float v = m_controller->get_control_out() ;

        //send command.
        m_command_->send_velocity_send(1,v*60.0/125.0);//send control value.

        if(!m_isCanRun)
        {
            m_command_->send_motor_stop(1);//stop motor.
            return;
        }
        cnt++;
    } while (1);
}
