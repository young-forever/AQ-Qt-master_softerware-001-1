#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "data_analysis.h"
#include <QThread>
#include "threadfromqthread.h"
#include "my_serialport.h"
#include "my_command.h"
#include <QObject>
#include <QDebug>
#include <QTimerEvent>
#include <QTimer>
#include "controlthread.h"
#include "MvCameraControl.h"
#include "cameraworkthread.h"

#define TIMER_TIMEOUT	(1)//set timer interrupt time. unit ms

#define PositionMode 0xd0
#define VelocityMode 0xc4
#define CurrentMode  0xc1

#define STOP_STATE 0X01
#define STOP_STATE 0X01

#define MAX_RPM 600
#define MAX_CURRENT 5000

#define STEP_PLANNER 0X01
#define S_POSITION_PLANNER 0X02

extern unsigned char PlanType;
extern bool ZVD_flag;

extern unsigned char MotorMode;

extern int S_cnt;
extern int R_cnt;

extern float p_theta;
extern float dp_theta;

extern float theta;
extern float position_x;
extern float d_theta;
extern float d_position_x;

extern float limit_velocity;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    float Xd_;

    basic_para* m_basic_para;

    Data_Analysis *Data_Analysis_;
    QTimer* m_pTimer;// timer
    controlthread* m_controlthread;//control thread.
    my_serialport* my_serial_;
    my_command* my_command_;
    ThreadFromQThread *m_thread;
    CameraWorkThread* m_cameraworkthread;//camera work thread.
    Circle_Queue<unsigned char*>* m_queue_camera;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    short getMotorNum(void);

private:
    Ui::MainWindow *ui;


public slots:
    void serial_ui_update(void);
    void S_R_ui_refresh(void);

    void sensor_data_ui_refresh(void);
    void AQ_error_warning_ui_refresh(void);
    void AQ_error_right_limit_warning_ui_refresh(void);
    void AQ_error_left_limit_warning_ui_refresh(void);


    void handleTimeout(void);

    void show_iamge_from_camera();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_sned_test_clicked();
    void on_Position_send_pushButton_released();
    void on_Velocity_send_pushButton_released();
    void on_Current_send_pushButton_released();

    void on_Stop_pushButton_released();
    void on_show_data_analysis_pushButton_released();
    void on_Zero_pushButton_released();
    void on_test_pushButton_released();
    void on_camera_pushButton_released();
    void on_save_image_pushButton_released();
    void on_Brake_pushButton_released();
    void on_Magnet_1_pushButton_released();
    void on_Magnet_2_pushButton_released();
    void on_PID_test_send_pushButton_released();

    void on_Limit_Velocity_send_pushButton_released();
    void on_read_next_image_pushButton_released();
};

#endif // MAINWINDOW_H
