#include "my_command.h"

using namespace std;

my_command::my_command(my_serialport* serialport) : my_serial_(serialport)
{



}

void my_command ::send_testback(void)
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

    my_serial_->my_serial_write(data,8);
}


int my_command :: send_consucf(void)
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

    int wlen = my_serial_->my_serial_write(data,8);
    cout<<"send shake hands command."<<endl;
    //    emit my_serial_->SR_ui_refresh_signal();
    return wlen;
}


//send break/close connection command.
int my_command :: send_bkcon(void)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}


//send motor mode choose command.
int my_command :: send_motor_mode_choose(const short motor_num, const unsigned char mode)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

//send motor mode choose command.
int my_command :: send_position_send(const short motor_num, float position_value)
{
    char data[8];
    int pdata = position_value*100000;

    data[0] = 0x56;
    data[1] = 0x2D;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

int my_command :: send_velocity_send(const short motor_num, float velocity_value)
{
    char data[8];
    int pdata = velocity_value*100000;

    data[0] = 0x56;
    data[1] = 0x4D;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;

}

int my_command :: send_current_send(const short motor_num, float current_value)
{
    char data[8];
    int pdata = current_value*100000;

    data[0] = 0x56;
    data[1] = 0x6D;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

int my_command :: send_limit_velocity_send(const short motor_num, float limit_v)
{
    char data[8];
    int pdata = limit_v*100000;

    data[0] = 0x56;
    data[1] = 0xAD;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

//send break/close connection command.
int my_command :: send_motor_stop(const short motor_num)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x8D;
    data[2] = 0x55;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

//send break/close connection command.
int my_command :: send_motor_run(const short motor_num)
{
    char data[8];

    data[0] = 0x56;
    data[1] = 0x9D;
    data[2] = 0x55;
    data[3] = 0X55;
    data[4] = 0x55;
    data[5] = 0x55;
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

int my_command :: send_camera_trigger(const int sample_time,const unsigned char flag)
{
    char data[8];
    int pdata = sample_time;

    data[0] = 0x56;
    data[1] = 0x0C;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = flag;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;

}

int my_command :: send_brake_control(const unsigned char flag)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;

}


int my_command :: send_magnet1_control(const unsigned char flag)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;

}


int my_command :: send_magnet2_control(const unsigned char flag)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;

}



int my_command :: send_remote(const unsigned char flag)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

int my_command :: send_reset_position(void)
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

//reuire length of l.
int my_command :: send_l_require()
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

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}


//debug function.
int my_command :: send_test_x_send(const short motor_num, float test_x_value)
{
    char data[8];
    int pdata = test_x_value*100000;

    data[0] = 0x56;
    data[1] = 0x44;
    data[2] = (unsigned char)(pdata & 0xff);
    data[3] = (unsigned char)(pdata>>8 & 0xff);
    data[4] = (unsigned char)(pdata>>16 & 0xff);
    data[5] = (unsigned char)(pdata>>24 & 0xff);
    data[6] = (unsigned char)motor_num;
    data[7] = 0x0D;

    int wlen = my_serial_->my_serial_write(data,8);
    return wlen;
}

