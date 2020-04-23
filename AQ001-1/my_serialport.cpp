#include "my_serialport.h"
#include "mainwindow.h"

using namespace std;

int my_serialport::my_serial_read(char* buffer,const int len)
{
    int rdlen;

    //    cout<<"11"<<endl;
    rdlen = read(fd, buffer, len);
    //    cout<<"44"<<endl;
    return rdlen;
}

int my_serialport::my_serial_write(const char* data, const int len)
{
    wlen = write(fd,data,len);
    return wlen;
}

//void my_serialport::get_buf(char* buffer)
//{
//    buffer = buf[0];
//}

void my_serialport::set_read_cnt(int cnt)
{
    read_cnt = cnt;
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

        //CAMERA TRIGGER OK!
    case 0x0C:
        CameraWorkFlag = true;
        break;

        //sensor data.
    case 0x0E:
        switch(re_command)
        {
        case 0X0E://theta
            id = (re_command & 0xf0) >> 4;
            memcpy(&data,re_data,4);
            pdata = data/100000.0;
            theta = pdata;
            emit sensor_data_ui_refresh_signal();
            break;

        case 0X2E://Position x.
            id = (re_command & 0xf0) >> 4;
            memcpy(&data,re_data,4);
            pdata = (float)data/100000.0;
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
void my_serialport::long_command_execute(const unsigned char re_command, const unsigned char* re_data)
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
    pdata = data/100000.0;
    position_x = pdata;

    //get d_position_x.
    temp_data[0] = re_data[4];
    temp_data[1] = re_data[5];
    temp_data[2] = re_data[6];
    temp_data[3] = re_data[7];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/100000.0;
    d_position_x = pdata;

    //get theta.
    temp_data[0] = re_data[8];
    temp_data[1] = re_data[9];
    temp_data[2] = re_data[10];
    temp_data[3] = re_data[11];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/100000.0;
    theta = pdata;
    //    cout<<"theta: "<<theta<<endl;

    //get d_theta.
    temp_data[0] = re_data[12];
    temp_data[1] = re_data[13];
    temp_data[2] = re_data[14];
    temp_data[3] = re_data[15];

    memcpy(&data,temp_data,4);//put recieve data to temp data.
    pdata = data/100000.0;
    d_theta = pdata;

    //    cout<<"dtheta: "<<d_theta <<"   "<<"theta"<<theta<<endl;
    emit sensor_data_ui_refresh_signal();
    //                                cout<<"lcommand;"<<endl;
}

my_serialport::my_serialport(QObject *parent) : QObject(parent)
{
    read_cnt = 2;

    const char *portname = "/dev/ttyUSB0";

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC );
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B2000000);

    //set_mincount(fd, 0);                /* set to pure timed read */

    /* simple output */
    wlen = write(fd, "Hello!\n", 7);
    if (wlen != 7) {
        printf("Error from write: %d, %d\n", wlen, errno);

    }
    tcdrain(fd);    /* delay for output */
}


my_serialport::~my_serialport()
{

}
