#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include "threadfromqthread.h"
#include <iostream>
#include <QTimerEvent>
#include<QMessageBox>
#include<QFileDialog>
#include<QDebug>
#include <QObject>
#include <QDialog>
#include <sstream>
#include <fstream>

float limit_velocity = 600;

bool ZVD_flag = false;
bool l_update_flag = false;
unsigned char MotorMode[2] = {NULL};

using namespace std;
bool stop_flag = true;
bool brake_flag = true;
bool magnet1_flag = true;
bool magnet2_flag = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  // m_thread()
{
    ui->setupUi(this);
    m_basic_para = new basic_para();
    m_planner_para = new planner_para();
    Data_Analysis_ = new Data_Analysis();
    m_sample_label = new sample_label(1000);

    //init qtimer
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));//similar to timer interrupts.
    m_pTimer->start(TIMER_TIMEOUT);//set timing interval.

    //creat serial.
    my_serial_ = new my_serialport(this);
    my_serial_->start();

    //open serial comm thread.
    //    m_thread = new ThreadFromQThread(this, my_serial_);
    //    m_thread->start();
    //    my_command_ = new my_command(my_serial_);

    //controller
    m_controlthread = new controlthread(my_serial_);//control thread.

    //open camera work thread,
    m_cameraworkthread = new CameraWorkThread(this);
    m_cameraworkthread->start();

    //image save thread.
    //    m_image_save_thread = new  image_save_thread(this,m_state_data);

    //connect list:
    connect(m_cameraworkthread, &CameraWorkThread::show_image, this,  &MainWindow::show_iamge_from_camera);//link camera image to ui.
    connect(m_cameraworkthread, &CameraWorkThread::refresh_camera_cnt_signal, this,  &MainWindow::refresh_camera_cnt);//link camera image to ui.

    connect(this, &MainWindow::ZV_tpye_send, m_controlthread,  &controlthread::ZV_tpye_recieve);//
    connect(this, &MainWindow::set_delat_T, m_controlthread,  &controlthread::set_delat_T);//
    connect(this, &MainWindow::set_plantype, m_controlthread,  &controlthread::set_plantype);//
    connect(m_cameraworkthread, &CameraWorkThread::image_path_save, this,  &MainWindow::save_label);//
    connect(m_controlthread, &controlthread::control_stop_signal, this,  &MainWindow::stop_sampling);//
    connect(m_controlthread, &controlthread::noimage_aotu_sample, this,  &MainWindow::save_label);//
    connect(this,  &MainWindow::open_noimage_sample_flag,m_controlthread, &controlthread::set_noimage_sample_flag);//
    connect(this,  &MainWindow::drop_flag_signal,m_controlthread, &controlthread::drop_flag_slot);//
    connect(m_controlthread, &controlthread::drop_weight_signal, this,  &MainWindow::drop_weight_slot);//

    connect(my_serial_, SIGNAL(SR_ui_refresh_signal()), this, SLOT(S_R_ui_refresh()));//link serial interup to ui.
    connect(my_serial_, SIGNAL(sensor_data_ui_refresh_signal()), this, SLOT(sensor_data_ui_refresh()));//link serial interup to ui.
    connect(my_serial_, SIGNAL(AQ_error_signal()), this, SLOT(AQ_error_warning_ui_refresh()));//aq_error
    connect(my_serial_, SIGNAL(AQ_error_Right_Limit_signal()), this, SLOT(AQ_error_right_limit_warning_ui_refresh()));//right limit error.
    connect(my_serial_, SIGNAL(AQ_error_Left_Limit_signal()), this, SLOT(AQ_error_left_limit_warning_ui_refresh()));//left limit error.
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool send_test_flag = false;
void MainWindow::handleTimeout(void)
{
    if(m_pTimer->isActive()){
        m_pTimer->start(TIMER_TIMEOUT);

        if(send_test_flag)
        {
            my_serial_->send_testback();//send test command.
            S_cnt++;
            S_R_ui_refresh();
        }
    }
}

void MainWindow::S_R_ui_refresh(void)
{
    QString S = "S:";
    S.append(QString::number(S_cnt));
    ui->label_send_cnt->setText(S);

    QString R = "R:";
    R.append(QString::number(R_cnt));
    ui->label_recieve_cnt->setText(R);
}


void MainWindow::sensor_data_ui_refresh(void)
{
    ui->lineEdit_X->setText(QString::number(position_x,'f',3));
    ui->lineEdit_theta->setText(QString::number(theta,'f',3));
    ui->lineEdit_dX->setText(QString::number(d_position_x,'f',3));
    ui->lineEdit_dtheta->setText(QString::number(d_theta,'f',3));

//    ui->lineEdit_roll->setText(QString::number(roll,'f',3));
//    ui->lineEdit_pitch->setText(QString::number(pitch,'f',3));
//    ui->lineEdit_yaw->setText(QString::number(yaw,'f',3));

//    ui->lineEdit_l->setText(QString::number(position_l,'f',3));
}


void MainWindow::AQ_error_warning_ui_refresh(void)
{
    m_controlthread->stopImmediately();
    QMessageBox::warning(this,tr("Warning:"),tr("some error happen of AQ, please restart device!"));
}


void MainWindow::AQ_error_right_limit_warning_ui_refresh(void)
{
    m_controlthread->stopImmediately();
    QMessageBox::warning(this,tr("Warning:"),tr("moving right limit, restart AQ!"));
}


void MainWindow::AQ_error_left_limit_warning_ui_refresh(void)
{
    m_controlthread->stopImmediately();
    QMessageBox::warning(this,tr("Warning:"),tr("moving left limit, restart AQ!"));
}

//void MainWindow::serial_ui_update(void)
//{
//    int convert_len = 8;
//    unsigned char high_hex;
//    unsigned char low_hex;
//    QString frame;
//    for (int i = 0; i < convert_len; i++)
//    {
//        high_hex = (unsigned char)data_test[i] >> 4;
//        low_hex  = (unsigned char)data_test[i] & 0x0F;

//        if (high_hex > 9)
//        {
//            high_hex = high_hex + '0' + 7;
//        }
//        else
//        {
//            high_hex = high_hex + '0';
//        }

//        if(low_hex > 9)
//        {
//            low_hex = low_hex + '0' + 7;
//        }
//        else
//        {
//            low_hex = low_hex + '0';
//        }

//        frame += high_hex;
//        frame += low_hex;
//        frame += " ";

//    }
//    ui->textEdit_serial->append(frame);
//}


void MainWindow::on_connect_pushButton_released()
{
    static bool conflag = false;

    if(conflag == false)
    {
        ui->connect_pushButton->setText("close");
        my_serial_->send_consucf();
        usleep(1000);
//        my_serial_->send_state_requeset_set(1);
    }
    else
    {
        ui->connect_pushButton->setText("connect");
        my_serial_->send_bkcon();
    }
    conflag = !conflag;
}

void MainWindow::on_pushButton_sned_test_clicked()
{
    if(send_test_flag == false)
    {
        ui->pushButton_sned_test->setText("send_stop");
    }
    else
    {
        ui->pushButton_sned_test->setText("send_test");
    }
    send_test_flag = !send_test_flag;
}

short MainWindow::getMotorNum(void)
{
    if(ui->Motor1_radioButton->isChecked())
    {
        return 1;
    }
    else if(ui->Motor2_radioButton->isChecked())
    {
        return 2;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please choose motor first!"));
        return -1;
        //error:please choose motor first!
    }
}

int grp_num = 0;
int image_get_cnt = 1;
bool camera_start_flag = false;
void MainWindow::on_Position_send_pushButton_released()
{
    if(ui->Position_expect_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please give the position data."));
        return;
    }
    short Motor_Num = getMotorNum();
    if(Motor_Num == -1)
    {
        return;
    }
    if(ui->Velocity_expect_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please set velocity limit first."));
        return;
    }
    else if(fabs(ui->Velocity_expect_lineEdit->text().toFloat())>300.0)
    {
        QMessageBox::warning(this,tr("Warning:"),tr("Velocity limit can't big than 300rpm."));
        return;
    }


    if(ui->Position_mode_radioButton->isChecked())
    {
        if(MotorMode[Motor_Num-1] != PositionMode){
            MotorMode[Motor_Num-1] = PositionMode;
            my_serial_->send_limit_velocity_send(Motor_Num,ui->Velocity_expect_lineEdit->text().toFloat());
            usleep(5000);
            my_serial_->send_motor_mode_choose(Motor_Num,MotorMode[Motor_Num-1]);
        }
        my_serial_->send_position_send(Motor_Num,ui->Position_expect_lineEdit->text().toFloat());
        ui->Stop_pushButton->setText("STOP");
        stop_flag = false;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please chose position mode first."));
        //error: please chose position mode first.
    }

    //    grp_num++;

    //    //save image:
    //    string str1 = "image/";//name of foder
    //    str1.append(to_string(grp_num));
    //    str1.append("/");

    //    m_lock.unlock();
    //    m_state_data->ClearQueue();
    //    m_lock.unlock();

    //    m_image_save_thread->set_save_dir(str1);
    //    m_image_save_thread->start();//open iamge saved thread.
}


void MainWindow::on_Velocity_send_pushButton_released()
{
    if(ui->Velocity_expect_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please give the Velocity data."));
        return;
    }
    short Motor_Num = getMotorNum();
    if(Motor_Num == -1)
    {
        return;
    }

    if(ui->Velocity_mode_radioButton->isChecked())
    {
        if(fabs(ui->Velocity_expect_lineEdit->text().toFloat())>MAX_RPM)
        {
            QMessageBox::warning(this,tr("Warning:"),tr("Set velocity too big!!"));
            return;
        }

        if(MotorMode[Motor_Num-1] != VelocityMode){
            MotorMode[Motor_Num-1] = VelocityMode;
            my_serial_->send_motor_mode_choose(Motor_Num,MotorMode[Motor_Num-1]);
        }
        my_serial_->send_velocity_send(Motor_Num,ui->Velocity_expect_lineEdit->text().toFloat());
        ui->Stop_pushButton->setText("STOP");
        stop_flag = false;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please chose velocity mode first."));
        //error: please chose velocity mode first.
    }
}

void MainWindow::on_Current_send_pushButton_released()
{
    if(ui->Current_expect_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please give the Current data."));
        return;
    }
    short Motor_Num = getMotorNum();
    if(Motor_Num == -1)
    {
        return;}

    if(ui->Current_mode_radioButton->isChecked())
    {
        if(fabs(ui->Current_expect_lineEdit->text().toFloat())>MAX_CURRENT)
        {
            QMessageBox::warning(this,tr("Warning:"),tr("Set current too big!!"));
            return;
        }

        if(MotorMode[Motor_Num-1] != CurrentMode){
            MotorMode[Motor_Num-1] = CurrentMode;
            my_serial_->send_motor_mode_choose(Motor_Num,MotorMode[Motor_Num-1]);
        }
        my_serial_->send_current_send(Motor_Num,ui->Current_expect_lineEdit->text().toFloat());
        ui->Stop_pushButton->setText("STOP");
        stop_flag = false;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning:"),tr(" please chose current mode first."));
        //error: please chose current mode first.
    }
}


void MainWindow::on_Stop_pushButton_released()
{
    if(stop_flag == false)
    {
        ui->Stop_pushButton->setText("RUN");
        my_serial_->send_motor_stop();
        stop_flag = true;
    }
    else
    {
        ui->Stop_pushButton->setText("STOP");
        my_serial_->send_motor_run();
        stop_flag = false;
    }
}

void MainWindow::on_show_data_analysis_pushButton_released()
{
    Data_Analysis_->show();
}

void MainWindow::on_Zero_pushButton_released()
{

}

//void MainWindow::on_test_pushButton_released()
//{
//    my_serial_->send_test_x_send(1,ui->Current_expect_lineEdit->text().toFloat());
//}


void MainWindow::show_iamge_from_camera(unsigned char *pImage)
{
    //get image and convert to pixmap.
    QImage img;
    img.loadFromData(pImage,2448*2048*4+2048,"jpeg");
    QPixmap pixmap = QPixmap::fromImage(img);

    //fit image to label window size.
    int i_width = ui->camer_label->width();
    int i_height = ui->camer_label->height();
    QPixmap fitpixmap = pixmap.scaled(i_width,i_height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    //show image.
    ui->camer_label->setPixmap(fitpixmap);
    ui->camer_label->show();
}


void MainWindow::refresh_camera_cnt(int camera_cnt)
{
    QString str1 = "read cnt:";
    str1.append(QString::number(camera_cnt));
    ui->camera_read_cnt_label->setText(str1);

    QString str2 = "trigger cnt:";
    //    str2.append(QString::number(camera_cnt_R));
    //    ui->camera_trigger_cnt_label->setText(str2);
}

void MainWindow::save_label(int path)
{
    QString label_stream=NULL;
    QMutexLocker locker(&m_lock);

    float vx = m_controlthread->get_target_vx();
    float vl = m_controlthread->get_target_vl();

    // define table title of csv
    if(path == 1)
    {
        label_stream = "image,x(mm),l(mm),theta(deg),dx(mm/s),dl(mm/s),dtheta(deg/s),Vx(mm/s),Vl(mm/s)";
        m_sample_label->add_label(label_stream.toStdString());
    }
    label_stream = QString::number(path)+","+\
            QString::number(position_x)+","+QString::number(position_l)+","+QString::number(theta)+","+\
            QString::number(d_position_x)+","+QString::number(d_position_l)+","+QString::number(d_theta)+","+\
            QString::number(vx)+","+QString::number(vl);

    bool ret = m_sample_label->add_label(label_stream.toStdString());
    if(ret == false)
    {
        // close sample process.
        on_sample_save_pushButton_released();
        QMessageBox::warning(this,tr("Warning:"),tr("label buffer full, sample process stop."));
    }
}

void MainWindow::stop_sampling(void)
{
    if(ui->auto_sample_checkBox->isChecked())
    {
        on_sample_save_pushButton_released();
    }
}


void MainWindow::drop_weight_slot(void)
{
    ui->drop_flag_checkBox->setChecked(false);
    //require length of l:
    my_serial_->send_l_require();
    while(l_update_flag == false)
    {
        usleep(5000);
        cout<<"waitting for l update."<<endl;
    }
    cout<<"l update ok!"<<endl;
    l_update_flag = false;

    float Ld_ = ui->floor_height_lineEdit->text().toFloat()/1000.0;
    //get plan type.
    unsigned char PlanType = S_POSITION_PLANNER;
    emit set_plantype(PlanType);

    // get ZV type:
    unsigned char zv_flag = 0x00;
    emit ZV_tpye_send(zv_flag);

    // get planner para:
    m_planner_para->Xd = position_x/1000.0;
    m_planner_para->Xc = position_x/1000.0;
    m_planner_para->Ld = Ld_;
    m_planner_para->Lc = position_l/1000.0;

    m_planner_para->Td = (Ld_ - position_l/1000.0)*20;

    m_controlthread->set_para(m_basic_para,m_planner_para);

    m_controlthread->start();//start control thread!
}


void MainWindow::on_camera_pushButton_released()
{
    static unsigned char flag = 1;
    int sample_time=100;
    if(ui->sample_frequence_lineEdit->text().toFloat()>0 && flag ==1)
    {
        sample_time = (int)(1000/ui->sample_frequence_lineEdit->text().toFloat());
        //        QMessageBox::warning(this,tr("info:"),tr(" camera open finished!"));
    }
    else if(flag == 1)
    {
        QMessageBox::warning(this,tr("warning:"),tr("please check sample frequency input!"));
        return;
    }

    if(flag == 1)
    {
        ui->camera_pushButton->setText("close camera");
    }
    if(flag == 0)
    {
        ui->camera_pushButton->setText("open camera");
    }
    my_serial_->send_camera_trigger(sample_time,flag);
    flag = !flag;
}

void MainWindow::on_save_image_pushButton_released()
{
    //    unsigned char* p;

    //    int cnt = m_queue_camera->QueueLength();

    //    for(int i=0;i<cnt;i++)
    //    {
    //        string str1 = "image/image";

    //        stringstream iamg_num;
    //        iamg_num<<m_queue_camera->QueueLength();
    //        str1.append(iamg_num.str());
    //        str1.append(".bmp");

    //        m_queue_camera->DeQueue(p);
    //        QImage img;
    //        img.loadFromData(p,2448*2048*4+2048,"bmp");
    //        img.save(str1.c_str(), "bmp");
    //    }
}


void MainWindow::on_Brake_pushButton_released()
{
    if(brake_flag == false)
    {
        ui->Brake_pushButton->setText("Brake on");
        my_serial_->send_brake_control((char)brake_flag);
        brake_flag = true;
    }
    else
    {
        ui->Brake_pushButton->setText("Brake off");
        my_serial_->send_brake_control((char)brake_flag);
        brake_flag = false;
    }
}

void MainWindow::on_Magnet_1_pushButton_released()
{
    if(magnet1_flag == false)
    {
        ui->Magnet_1_pushButton->setText("Magnet_1 on");
        my_serial_->send_magnet1_control((char)magnet1_flag);
        magnet1_flag = true;
    }
    else
    {
        ui->Magnet_1_pushButton->setText("Magnet_1 off");
        my_serial_->send_magnet1_control((char)magnet1_flag);
        magnet1_flag = false;
    }
}

void MainWindow::on_Magnet_2_pushButton_released()
{
    if(magnet2_flag == false)
    {
        ui->Magnet_2_pushButton->setText("Magnet_2 on");
        my_serial_->send_magnet2_control((char)magnet2_flag);
        magnet2_flag = true;
    }
    else
    {
        ui->Magnet_2_pushButton->setText("Magnet_2 off");
        my_serial_->send_magnet2_control((char)magnet2_flag);
        magnet2_flag = false;
    }
}

void MainWindow::on_SC_send_pushButton_released()
{
    static bool open_flag = false;

    if(open_flag == false)
    {
        ui->SC_send_pushButton->setText("stop");
        open_flag = true;

        //require length of l:
        my_serial_->send_l_require();
        while(l_update_flag == false)
        {
            usleep(5000);
            cout<<"waitting for l update."<<endl;
        }
        ui->lineEdit_l->setText(QString::number(position_l,'f',3));

        cout<<"l update ok!"<<endl;
        l_update_flag = false;

        if(!(ui->SC_Ld_lineEdit->text().isEmpty()) && !(ui->SC_Td_lineEdit->text().isEmpty()) && !(ui->SC_Xd_lineEdit->text().isEmpty()))
        {

            float Xd_ = ui->SC_Xd_lineEdit->text().toFloat()/1000.0;
            float Ld_ = ui->SC_Ld_lineEdit->text().toFloat()/1000.0;

            float t_d = ui->SC_Td_lineEdit->text().toFloat();

            //get plan type.
            unsigned char PlanType = S_POSITION_PLANNER;
            emit set_plantype(PlanType);

            // get delta_T;
            if(ui->ZV_radioButton->isChecked() || ui->ZVD_radioButton->isChecked() || ui->ZVDD_radioButton->isChecked())
            {
                if(ui->delta_T_lineEdit->text().isEmpty())
                {
                    QMessageBox::warning(this,tr("warning:"),tr("please set value of delta_T!"));
                    return;
                }
                else
                {
                    if(ui->delta_T_lineEdit->text().toFloat()>0)
                    {
                        float T = ui->delta_T_lineEdit->text().toFloat();
                        emit set_delat_T(T);
                    }
                    else
                    {
                        QMessageBox::warning(this,tr("warning:"),tr("delta_T value must in range of 0~20s!"));
                        return;
                    }
                }
            }

            // get ZV type:
            unsigned char zv_flag;
            if(ui->ZV_radioButton->isChecked())
            {
                zv_flag = ZV_PLANN_TYPE;
            }
            else if(ui->ZVD_radioButton->isChecked())
            {
                zv_flag = ZVD_PLANN_TYPE;
            }
            else if(ui->ZVDD_radioButton->isChecked())
            {
                zv_flag = ZVDD_PLANN_TYPE;
            }
            else
            {
                zv_flag = 0x00;
            }
            emit ZV_tpye_send(zv_flag);

            // set drop flag:
            emit drop_flag_signal(ui->drop_flag_checkBox->isChecked());

            // get pid value:
            Kp = ui->Kp_lineEdit->text().toFloat();
            Ki = ui->Ki_lineEdit->text().toFloat();
            Kd = ui->Kd_lineEdit->text().toFloat();

            // get planner para:
            m_planner_para->Xd = Xd_;
            m_planner_para->Xc = position_x/1000.0;
            m_planner_para->Ld = Ld_;
            m_planner_para->Lc = position_l/1000.0;
            m_planner_para->Td = t_d;

            m_controlthread->set_para(m_basic_para,m_planner_para);

            if(ui->auto_sample_checkBox->isChecked())
            {
                on_sample_save_pushButton_released();
            }

            m_controlthread->start();//start control thread!
        }
        else
        {
            QMessageBox::warning(this,tr("Warning:"),tr(" please set necessary paraments!"));
            return;
        }

    }
    else
    {
        ui->SC_send_pushButton->setText("start");
        m_controlthread->stopImmediately();

        open_flag = false;

        return;
    }
}

void MainWindow::on_Vx_send_pushButton_released()
{
    static bool open_flag = false;

    if(open_flag == false)
    {
        ui->Vx_send_pushButton->setText("stop");
        open_flag = true;

        if(!(ui->Vx_lineEdit->text().isEmpty()) && !(ui->Vx_t_lineEdit->text().isEmpty()))
        {
            float Vx = ui->Vx_lineEdit->text().toFloat()/1000.0;
            float t_d = ui->Vx_t_lineEdit->text().toFloat();

            //get plan type.
            unsigned char PlanType = STEP_VELOCITY_PLANNER;
            emit set_plantype(PlanType);

            // get planner para:
            m_planner_para->Xd = Vx;
            m_planner_para->Xc = 0;
            m_planner_para->Ld = position_l/1000.0;
            m_planner_para->Lc = position_l/1000.0;
            m_planner_para->Td = t_d;

            m_controlthread->set_para(m_basic_para,m_planner_para);

            if(ui->auto_sample_checkBox->isChecked())
            {
                on_sample_save_pushButton_released();
            }

            m_controlthread->start();//start control thread!
        }
        else
        {
            QMessageBox::warning(this,tr("Warning:"),tr(" please set necessary paraments!"));
            return;
        }
    }
    else
    {
        ui->Vx_send_pushButton->setText("start");
        m_controlthread->stopImmediately();

        open_flag = false;

        return;
    }
}

void MainWindow::on_Limit_Velocity_send_pushButton_released()
{
    short Motor_Num = getMotorNum();

    MotorMode[Motor_Num-1] = PositionMode;
    limit_velocity = ui->Limit_velocity_lineEdit->text().toFloat();
    my_serial_->send_limit_velocity_send(Motor_Num,limit_velocity);
}


void MainWindow::on_remote_switch_pushButton_released()
{
    static bool remote_flag = false;
    if(remote_flag==false)
    {
        MotorMode[0] = VelocityMode;
        MotorMode[1] = VelocityMode;
        ui->remote_switch_pushButton->setText("remote off");
    }
    else
    {
        ui->remote_switch_pushButton->setText("remote on");
    }
    remote_flag = !remote_flag;
    my_serial_->send_remote((unsigned char)remote_flag);

}

void MainWindow::on_reset_position_pushButton_released()
{
    my_serial_-> send_reset_position();
}

void MainWindow::on_sample_save_pushButton_released()
{
    static bool open_flag = false;
    if(open_flag==false)
    {
        if(ui->sample_name_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning:"),tr(" please input sample folder name!"));
            return;
        }

        // define save dir
        sample_dir = "image/";
        QString chdir = ui->sample_name_lineEdit->text();
        sample_dir.append(chdir.toLatin1().toStdString());
        sample_dir.append("/");

        // create dir
        QDir dir;
        dir.mkdir(sample_dir.c_str());//creat floder.

        //  open camera save thread
        if(ui->image_label_checkBox->isChecked())
        {
            m_cameraworkthread->set_save_dir(sample_dir);
            m_cameraworkthread->open_iamge_save();
        }
        else
        {
            save_label(1);
            emit open_noimage_sample_flag();
        }

        ui->sample_save_pushButton->setText("close");

        // define csv name
        sample_dir.append(chdir.toLatin1().toStdString());
        sample_dir.append(".csv");
        m_sample_label->set_save_dir(sample_dir);
    }
    else
    {
        ui->sample_save_pushButton->setText("sample");
        if(ui->image_label_checkBox->isChecked())
        {
            m_cameraworkthread->close_image_save();
        }
        //sample finished, print label data:
        m_sample_label->save_label();
    }
    open_flag = !open_flag;
}


void MainWindow::on_Driver_Velocity_PID_Set_pushButton_released()
{
    if(ui->Driver_Velocity_Kp_lineEdit->text().isEmpty() || ui->Driver_Velocity_Ki_lineEdit->text().isEmpty() || ui->Driver_Velocity_Kd_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning:"),tr(" please input PID para!"));
        return;
    }

    // send pid
    //    my_serial_->send

}
