#include "threadfromqthread.h"

using namespace std;


ThreadFromQThread::ThreadFromQThread(QObject* par, my_serialport* serialport) : QThread(par),my_serial_(serialport)
{
    m_isCanRun = true;
}

ThreadFromQThread::~ThreadFromQThread()
{
}

void ThreadFromQThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

char data_test[8]={0};//暂时只用到一位值
char buffer[4096] = {0};

//该线程执行串口数据接受任务
void ThreadFromQThread::run()
{
    static int addr;


    do {
        int rdlen;

//        cout<<"before"<<endl;
        rdlen = my_serial_->my_serial_read(data_test,1);//read my serial.

//        cout<<"after"<<endl;
//        for(int i=0;i<1;i++)
//        {
//            cout<<hex<<" "<<(int)(unsigned char)data_test[i];
//        }
//        cout<<endl;

        //***send signal to ui.
        if (rdlen > 0) {
            //  emit serial_ok_signal();//
        }
        else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        }

        /* repeat read to get full message */

        //*** put data to buffer
        memcpy(buffer+addr,data_test,1);
        addr = addr +1;
        QByteArray buff_arry = QByteArray(buffer,addr);

        //*** buffer data process.

        while(buff_arry.size() >= 8)//juge whether the
        {
            //short command analysis.
            if(buff_arry.data()[0] == 0x56)//short command.
            {
                if(buff_arry.data()[7] == 0x0D)
                {
                    unsigned char re_command = buff_arry[1];//功能码

                    static int cnt_test= 0;
                    cnt_test++;
                    //                    cout<<"count number:"<<cnt_test<<endl;

                    buff_arry.remove(0,2);//移除帧头和功能码

                    QByteArray re_data = buff_arry.left(5);//指令数据位, 6位Byte

                    my_serial_->command_execute(re_command, (unsigned char*)re_data.data());//执行指令解析操作

                    buff_arry.remove(0,6);//移除执行过的数据帧
                }
                else
                {
                    buff_arry.remove(0,1);//移除执行过的数据帧
                }
            }
            //** long command analysis.
            else if(buff_arry.data()[0] == 0x7F)//long command
            {
                if( buff_arry.size() < 32)
                {
                    break;
                }

                if(buff_arry.data()[31] == 0x0D)
                {
                    unsigned char re_command = buff_arry[1];//功能码

                    static int cnt_test= 0;
                    cnt_test++;
                    //                    cout<<"count number:"<<cnt_test<<endl;

                    buff_arry.remove(0,2);//移除帧头和功能码

                    QByteArray long_re_data = buff_arry.left(28);//指令数据位, 6位Byte

                    my_serial_->long_command_execute(re_command, (unsigned char*)long_re_data.data());//执行指令解析操作

                    buff_arry.remove(0,6);//移除执行过的数据帧
                }
                else
                {
                    buff_arry.remove(0,1);//移除执行过的数据帧
                }
            }
            else
            {
                buff_arry.remove(0,1);
            }
        }

        addr = buff_arry.size();
        memcpy(buffer,buff_arry,addr);

        QMutexLocker locker(&m_lock);

        if(!m_isCanRun)
        {
            return;
        }
    } while (1);
}

