#ifndef MY_COMMAND_H
#define MY_COMMAND_H
#include "my_serialport.h"


class my_command
{

private:
    my_serialport* my_serial_;



public:
    my_command(my_serialport* serialport);
    ~my_command();

    //COMM_send function.
    int send_consucf(void);//connection succseful sendback.
    int send_bkcon(void);
    void send_testback(void);

    int send_motor_mode_choose(const short motor_num,const unsigned char mode);
    int send_position_send(const short motor_num, float position_value);
    int send_velocity_send(const short motor_num, float velocity_value);
    int send_current_send(const short motor_num, float current_value);
    int send_motor_stop(const short motor_num);
    int send_motor_run(const short motor_num);
    int send_camera_trigger(const unsigned char flag);
    int send_brake_control(const unsigned char flag);
    int send_magnet1_control(const unsigned char flag);
    int send_magnet2_control(const unsigned char flag);

    //debug:
    int send_test_x_send(const short motor_num, float test_x_value);
};

#endif // MY_COMMAND_H
