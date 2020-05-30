#include "image_save_thread.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include"qdir.h"
using namespace std;

image_save_thread::image_save_thread(QObject* par, Circle_Queue<unsigned char*> *m_queue) : QThread(par),m_queue_camera(m_queue)
{
    img = new QImage();

    m_isCanRun = true;

}

image_save_thread::~image_save_thread()
{

}

void image_save_thread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

void image_save_thread::set_save_dir(string dir)
{
    save_dir = dir;
}

void image_save_thread::run()
{
    m_isCanRun = true;
    string iamge_dir;
    int image_cnt=1;

    unsigned char* p;

    do {
        m_lock.lock();
        bool ret = m_queue_camera->DeQueue(p);


        if(ret)
        {
            /*********  save images  *********/
            stringstream iamg_num;
            iamg_num<<image_cnt;
            iamge_dir = save_dir;
            iamge_dir.append(iamg_num.str());
            img->loadFromData(p,2448*2048*4+2048,"JPEG");
            img->save(iamge_dir.c_str(), "JPG");

            image_cnt++;
        }
        m_lock.unlock();

        QMutexLocker locker(&m_lock);

        if(!m_isCanRun && m_queue_camera->QueueEmpty())
        {
            cout<<"stop!!"<<endl;
            return;
        }
    } while (1);
}
