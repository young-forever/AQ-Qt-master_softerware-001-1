#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "data_analysis.h"
#include <QThread>
//#include "threadfromqthread.h"
#include "my_serialport.h"
//#include "my_command.h"
#include <QObject>
#include <QDebug>
#include <QTimerEvent>
#include <QTimer>
#include "controlthread.h"
#include "MvCameraControl.h"
#include "cameraworkthread.h"
#include "image_save_thread.h"
#include "sample_label.h"
#include <iostream>
#include <sstream>
#include <fstream>


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
#define NANKAI_PLANNER 0X03
#define T_VELOCITY_PLANNER 0X04
#define STEP_VELOCITY_PLANNER 0X05

extern float limit_velocity;

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMutex m_lock;
    basic_para* m_basic_para;
    planner_para* m_planner_para;

    Data_Analysis *Data_Analysis_;
    QTimer* m_pTimer;// timer
    controlthread* m_controlthread;//control thread.
    my_serialport* my_serial_;

    CameraWorkThread* m_cameraworkthread;//camera work thread.
    string sample_dir;
    sample_label *m_sample_label;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    short getMotorNum(void);

private:
    Ui::MainWindow *ui;

public slots:
    //    void serial_ui_update(void);
    void S_R_ui_refresh(void);
    void sensor_data_ui_refresh(void);
    void AQ_error_warning_ui_refresh(void);
    void AQ_error_right_limit_warning_ui_refresh(void);
    void AQ_error_left_limit_warning_ui_refresh(void);
    void handleTimeout(void);
    void show_iamge_from_camera(unsigned char *pImage);
    void refresh_camera_cnt(int camera_cnt);
    void save_label(int path);

    void stop_sampling(void);
    void drop_weight_slot(void);

private slots:
    void on_connect_pushButton_released();
    void on_pushButton_sned_test_clicked();
    void on_Position_send_pushButton_released();
    void on_Velocity_send_pushButton_released();
    void on_Current_send_pushButton_released();

    void on_Stop_pushButton_released();
    void on_show_data_analysis_pushButton_released();
    void on_Zero_pushButton_released();
    void on_camera_pushButton_released();
    void on_save_image_pushButton_released();
    void on_Brake_pushButton_released();
    void on_Magnet_1_pushButton_released();
    void on_Magnet_2_pushButton_released();

    void on_Limit_Velocity_send_pushButton_released();
    void on_remote_switch_pushButton_released();
    void on_reset_position_pushButton_released();
    void on_SC_send_pushButton_released();
    void on_sample_save_pushButton_released();

    void on_Vx_send_pushButton_released();

    void on_Driver_Velocity_PID_Set_pushButton_released();

Q_SIGNALS:
    void ZV_tpye_send(const unsigned char zv_type);
    void set_delat_T(const float T);
    void set_plantype(const unsigned char plantype);
    void open_noimage_sample_flag(void);
    void drop_flag_signal(bool flag);
};

#endif // MAINWINDOW_H
