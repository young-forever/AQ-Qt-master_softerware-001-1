#ifndef MY_SERIALPORT_H
#define MY_SERIALPORT_H
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <QObject>
#include <QMutex>
#include <QThread>

extern bool ZVD_flag;
extern bool l_update_flag;

extern unsigned char MotorMode[2];

extern int S_cnt;
extern int R_cnt;

extern float p_theta;
extern float dp_theta;
extern float p_x;
extern float dp_x;


extern float theta;     //deg
extern float position_x; //mm
extern float d_theta;//deg
extern float d_position_x;//mm/s
extern float position_l;  //mm
extern float d_position_l;  //mm/s


extern float roll;
extern float pitch;
extern float yaw;

class my_serialport : public QThread
{
    Q_OBJECT

public:
    explicit my_serialport(QObject *parent);
    ~my_serialport();

private:
    const char *portname;
    int fd;
    int wlen;
    QMutex m_lock;
    bool m_isCanRun;
    int decimal_size;

public slots:
    void stopImmediately();

public:
    void run();

    int my_serial_read(char* buffer,const int len);
    int my_serial_write(const char* data,const int len);
    int set_interface_attribs(int fd, int speed);
    void set_mincount(int fd, int mcount);

    void command_execute(const unsigned char re_command, const unsigned char* re_data);// 指令解析函数
    void long_command_execute(const unsigned char* re_data);//long command analysis.

    // command list:
    int send_consucf(void);//connection succseful sendback.
    int send_bkcon(void);
    int send_testback(void);
    int send_state_requeset_set(const unsigned char flag);
    int send_motor_mode_choose(const short motor_num,const unsigned char mode);
    int send_position_send(const short motor_num, float position_value);
    int send_velocity_send(const short motor_num, float velocity_value);
    int send_current_send(const short motor_num, float current_value);
    int send_limit_velocity_send(const short motor_num, float limit_v);
    int send_motor_stop(void);
    int send_motor_run(void);
    int send_camera_trigger(const int sample_time,const unsigned char flag);
    int send_brake_control(const unsigned char flag);
    int send_magnet1_control(const unsigned char flag);
    int send_magnet2_control(const unsigned char flag);
    int send_remote(const unsigned char flag);
    int send_reset_position(void);
    int send_l_require(void);
    int send_driver_velocity_Kp(const unsigned char Number,const short Kp);
    int send_driver_velocity_Ki(const unsigned char Number,const short Ki);
    int send_driver_velocity_Kd(const unsigned char Number,const short Kd);

Q_SIGNALS:
    void SR_ui_refresh_signal(void);
    void sensor_data_ui_refresh_signal(void);

    void AQ_error_signal(void);
    void AQ_error_Right_Limit_signal(void);
    void AQ_error_Left_Limit_signal(void);

    void serial_ok_signal(void);
};

#endif //
