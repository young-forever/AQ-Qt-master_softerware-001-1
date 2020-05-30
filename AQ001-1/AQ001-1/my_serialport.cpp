#include "my_serialport.h"

int S_cnt = 0;
int R_cnt = 0;

float p_theta =0.0;//image synchronous theta.
float dp_theta =0.0;
float p_x = 0.0;
float dp_x = 0.0;

float theta = 0.0;
float d_theta = 0.0;
float position_x = 0.0;
float d_position_x = 0.0;
float position_l = 0.0;  //mm
float d_position_l = 0.0;  //mm/s

float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;

using namespace std;

void my_serialport::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;

    decimal_size = 1000;
}

int my_serialport::my_serial_read(char* buffer,const int len)
{
    int rdlen;
    rdlen = read(fd, buffer, len);
    return rdlen;
}

int my_serialport::my_serial_write(const char* data, const int len)
{
    wlen = write(fd,data,len);
    return wlen;
}

int my_serialport::set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void my_serialport::set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}

//short command analysis.
void my_serialport::command_execute(const unsigned char re_command, const unsigned char* re_data)
{

    int data=0;
    float pdata=0;
    short id;//number of motor.

    switch(re_command & 0x0F)
    {

    case 0x00:  //serial port connect ok.
        //SerialConSucDisFlag = 0;

        //        if(!state_.getflag(serialconsucf))
        //        {
        //my_command_consucf(); //answer back connect ok command.
        //state_.setflag(serialconsucf);
        //        }
        break;
    case 0x02:
        id = (re_command & 0xf0) >> 4;//get high byte as number of motor.
        memcpy(&data,re_data,4);//put recieve data to temp data.
        pdata = data/100000.0;
        break;

        //test command recieve.
    case 0x0b:
        R_cnt++;
        emit SR_ui_refresh_signal();
        break;

        //get length of l.
    case 0x0C:
        switch(re_command)
        {
        case 0XCC:
            id = (re_command & 0xf0) >> 4;
            memcpy(&data,re_data,4);
            pdata = data/decimal_size;
            position_l = pdata;

            l_update_flag = true;
            emit sensor_data_ui_refresh_signal();
            break;
        }

        //sensor data.
    case 0x0E:
        switch(re_command)
        {
        case 0X0E://theta
            id = (re_command & 0xf0) >> 4;
            memcpy(&data,re_data,4);
            pdata = data/decimal_size;
            theta = pdata;
            emit sensor_data_ui_refresh_signal();
            break;

        case 0X2E://Position x.
            id = (re_command & 0xf0) >> 4;
            memcpy(&data,re_data,4);
            pdata = (float)data/decimal_size;
            position_x = pdata;
            emit sensor_data_ui_refresh_signal();
            break;
        }
        break;

        //error warning.
    case 0x0F:
        switch(re_data[0])
        {
        case 0X00://error flag.
            emit AQ_error_signal();
            break;
        case 0X02://error flag.
            emit AQ_error_Right_Limit_signal();
            break;
        case 0X03://error flag.
            emit AQ_error_Left_Limit_signal();
            break;
        }

        break;
    }
}

//long command analysis.
void my_serialport::long_command_execute(const unsigned char* re_data)
{

    unsigned char temp_data[4];
    int data=0;
    float pdata=0;
    //    short id;//number of motor.

    //get position_x.
    temp_data[0] = re_data[0];
    temp_data[1] = re_data[1];
    temp_data[2] = re_data[2];
    temp_data[3] = re_data[3];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/decimal_size;
    position_x = pdata;

    //get d_position_x.
    temp_data[0] = re_data[4];
    temp_data[1] = re_data[5];
    temp_data[2] = re_data[6];
    temp_data[3] = re_data[7];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/decimal_size;
    d_position_x = pdata;

    //get theta.
    temp_data[0] = re_data[8];
    temp_data[1] = re_data[9];
    temp_data[2] = re_data[10];
    temp_data[3] = re_data[11];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/decimal_size;
    theta = pdata;

    //get d_theta.
    temp_data[0] = re_data[12];
    temp_data[1] = re_data[13];
    temp_data[2] = re_data[14];
    temp_data[3] = re_data[15];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/decimal_size;
    d_theta = pdata;

//    //get roll.
//    temp_data[0] = re_data[16];
//    temp_data[1] = re_data[17];
//    temp_data[2] = re_data[18];
//    temp_data[3] = re_data[19];

//    memcpy(&data,temp_data,4);//put recieve data to temp data.
//    pdata = data/decimal_size;
//    roll = pdata;

//    //get pitch.
//    temp_data[0] = re_data[20];
//    temp_data[1] = re_data[21];
//    temp_data[2] = re_data[22];
//    temp_data[3] = re_data[23];

//    memcpy(&data,temp_data,4);//put recieve data to temp data.
//    pdata = data/decimal_size;
//    pitch = pdata;


//    //get yaw.
//    temp_data[0] = re_data[24];
//    temp_data[1] = re_data[25];
//    temp_data[2] = re_data[26];
//    temp_data[3] = re_data[27];

//    memcpy(&data,temp_data,4);//put recieve data to temp data.
//    pdata = data/decimal_size;
//    yaw = pdata;

    //get l length.
//    temp_data[0] = re_data[28];
//    temp_data[1] = re_data[29];
//    temp_data[2] = re_data[30];
//    temp_data[3] = re_data[31];

//    memcpy(&data,temp_data,4);//put recieve data to temp data.
//    pdata = data/100000.0;
//    position_l = pdata;

    emit sensor_data_ui_refresh_signal();
}

my_serialport::my_serialport(QObject *parent) : QThread(parent)
{
    decimal_size = 1000;


    // set serial fd.
    const char *portname = "/dev/ttyUSB0";
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC );
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B2000000);
    tcdrain(fd);    /* delay for output */

    m_isCanRun = true;
}

my_serialport::~my_serialport()
{

}

//command list:

int my_serialport ::send_testback(void)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x0B;
    data[2] = 0x01;
    data[3] = 0X02;
    data[4] = 0x03;
    data[5] = 0x04;
    data[6] = 0x05;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}


int my_serialport :: send_consucf(void)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x00;
    data[2] = 0x01;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    cout<<"send shake hands command."<<endl;
    //    emit my_serial_->SR_ui_refresh_signal();
    return wlen;
}


//send break/close connection command.
int my_serialport :: send_bkcon(void)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}


//send break/close connection command.
int my_serialport :: send_state_requeset_set(const unsigned char flag)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0xEC;
    data[2] = flag;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);

    cout<<"send requeset set ok!"<<endl;
    return wlen;
}


//send motor mode choose command.
int my_serialport :: send_motor_mode_choose(const short motor_num, const unsigned char mode)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x0D;
    data[2] = mode;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

//send motor mode choose command.
int my_serialport :: send_position_send(const short motor_num, float position_value)
{
    char data[8];
    int pdata = position_value*decimal_size;

    data[0] = 0x56;
    data[1] = 0x2D;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

int my_serialport :: send_velocity_send(const short motor_num, float velocity_value)
{
    char data[8];
    int pdata = velocity_value*decimal_size;

    data[0] = 0x56;
    data[1] = 0x4D;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;

}

int my_serialport :: send_current_send(const short motor_num, float current_value)
{
    char data[8];
    int pdata = current_value*decimal_size;

    data[0] = 0x56;
    data[1] = 0x6D;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

int my_serialport :: send_limit_velocity_send(const short motor_num, float limit_v)
{
    char data[8];
    int pdata = limit_v*decimal_size;

    data[0] = 0x56;
    data[1] = 0xAD;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

//change mode to stop state
int my_serialport :: send_motor_stop()
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x8D;
    data[2] = 0x55;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

//change mode to running state
int my_serialport :: send_motor_run(void)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x9D;
    data[2] = 0x55;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

int my_serialport :: send_camera_trigger(const int sample_time,const unsigned char flag)
{
    char data[8];
    int pdata = sample_time*decimal_size;

    data[0] = 0x56;
    data[1] = 0x0C;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = flag;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;

}

int my_serialport :: send_brake_control(const unsigned char flag)
{
    char data[8];
    data[0] = 0x56;
    data[1] = 0x2C;
    data[2] = flag;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;

}


int my_serialport :: send_magnet1_control(const unsigned char flag)
{
    char data[8];
    data[0] = 0x56;
    data[1] = 0x4C;
    data[2] = flag;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;

}


int my_serialport :: send_magnet2_control(const unsigned char flag)
{
    char data[8];
    data[0] = 0x56;
    data[1] = 0x6C;
    data[2] = flag;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;

}



int my_serialport :: send_remote(const unsigned char flag)
{
    char data[8];
    data[0] = 0x56;
    data[1] = 0x8C;
    data[2] = flag;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

int my_serialport :: send_reset_position(void)
{
    char data[8];
    data[0] = 0x56;
    data[1] = 0xAC;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

//reuire length of l.
int my_serialport :: send_l_require()
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0xCC;
    data[2] = 0x55;
    data[3] = 0x55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = 0x55;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}

int my_serialport :: send_driver_velocity_Kp(const unsigned char Number,const short Kp)
{
    char data[8];
    int pdata = Kp*decimal_size;

    data[0] = 0x56;
    data[1] = 0x04;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = Number;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}


int my_serialport :: send_driver_velocity_Ki(const unsigned char Number,const short Ki)
{
    char data[8];
    int pdata = Ki*decimal_size;

    data[0] = 0x56;
    data[1] = 0x14;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = Number;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}


int my_serialport :: send_driver_velocity_Kd(const unsigned char Number,const short Kd)
{
    char data[8];
    int pdata = Kd*decimal_size;

    data[0] = 0x56;
    data[1] = 0x24;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = Number;
    data[7] = 0x0D;

    int wlen = my_serial_write(data,8);
    return wlen;
}


//该线程执行串口数据接受任务
void my_serialport::run()
{
    static char buffer[4096] = {0};

    static int addr;
    char data_temp[2] = {0};//暂时只用到一位值

    do {
        int rdlen;

        rdlen = my_serial_read(data_temp,1);//read my serial.

        //***send signal to ui.
        if (rdlen > 0) {
            //  emit serial_ok_signal();//
        }
        else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        }

        //*** put data to buffer
        memcpy(buffer+addr,data_temp,1);
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

                    command_execute(re_command, (unsigned char*)re_data.data());//执行指令解析操作

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
                if( buff_arry.size() < 19)
                {
                    break;
                }

                if(buff_arry.data()[18] == 0x0D)
                {
                    //unsigned char re_command = buff_arry[1];//功能码

                    static int cnt_test= 0;
                    cnt_test++;
                    //                    cout<<"count number:"<<cnt_test<<endl;

                    buff_arry.remove(0,2);//移除帧头和功能码

                    QByteArray long_re_data = buff_arry.left(16);//指令数据位, 6位Byte

                    long_command_execute((unsigned char*)long_re_data.data());//执行指令解析操作

                    buff_arry.remove(0,17);//移除执行过的数据帧
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
