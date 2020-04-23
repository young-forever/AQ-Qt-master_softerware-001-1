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

class my_serialport : public QObject
{
    Q_OBJECT

public:
    explicit my_serialport(QObject *parent=0);
    ~my_serialport();

private:
    const char *portname;
    int fd;
    int wlen;
    int read_cnt;

public:
    //    void get_buf(char* buffer);
    void set_read_cnt(int cnt);
    int my_serial_read(char* buffer,const int len);
    int my_serial_write(const char* data,const int len);

    int set_interface_attribs(int fd, int speed);
    void set_mincount(int fd, int mcount);

    void command_execute(const unsigned char re_command, const unsigned char* re_data);// 指令解析函数
    void long_command_execute(const unsigned char re_command, const unsigned char* re_data);//long command analysis.

Q_SIGNALS:
    void SR_ui_refresh_signal(void);
    void sensor_data_ui_refresh_signal(void);

    void AQ_error_signal(void);
    void AQ_error_Right_Limit_signal(void);
    void AQ_error_Left_Limit_signal(void);


};


#endif // MY_SERIALPORT_H
