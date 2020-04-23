#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
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


int S_cnt = 0;
int R_cnt = 0;

float p_theta =0.0;//image synchronous theta.
float dp_theta =0.0;

float theta = 0.0;
float d_theta = 0.0;
float position_x = 0.0;
float d_position_x = 0.0;
float limit_velocity = 600;

unsigned char PlanType = NULL;
bool ZVD_flag = false;
unsigned char MotorMode = NULL;

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

    Xd_ = 0;


    m_basic_para = new basic_para();

    Data_Analysis_ = new Data_Analysis();

    m_queue_camera = new Circle_Queue<unsigned char*>(1000);

    //create serialport read thread.

    //init qtimer
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));//similar to timer interrupts.
    m_pTimer->start(TIMER_TIMEOUT);//set timing interval.

    //creat serial.
    my_serial_ = new my_serialport(this);

    //open serial comm thread.
    m_thread = new ThreadFromQThread(this, my_serial_);
    m_thread->start();
    my_command_ = new my_command(my_serial_);

    //controller
    m_controlthread = new controlthread(my_command_);//control thread.

    //open camera work thread,
    m_cameraworkthread = new CameraWorkThread(this);
    m_cameraworkthread->start();

    connect(m_cameraworkthread, SIGNAL(show_image()), this, SLOT(show_iamge_from_camera()));//link camera image to ui.

    connect(m_thread, SIGNAL(serial_ok_signal()), this, SLOT(serial_ui_update()));//link serial interup to ui.
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
            my_command_->send_testback();//send test command.
            S_cnt++;
            S_R_ui_refresh();
            //            cout<<S_cnt<<endl;
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

void MainWindow::serial_ui_update(void)
{
    int convert_len = 8;
    unsigned char high_hex;
    unsigned char low_hex;
    QString frame;
    for (int i = 0; i < convert_len; i++)
    {
        high_hex = (unsigned char)data_test[i] >> 4;
        low_hex  = (unsigned char)data_test[i] & 0x0F;

        if (high_hex > 9)
        {
            high_hex = high_hex + '0' + 7;
        }
        else
        {
            high_hex = high_hex + '0';
        }

        if(low_hex > 9)
        {
            low_hex = low_hex + '0' + 7;
        }
        else
        {
            low_hex = low_hex + '0';
        }

        frame += high_hex;
        frame += low_hex;
        frame += " ";

    }
    ui->textEdit_serial->append(frame);
}

void MainWindow::on_pushButton_clicked()
{
    static bool conflag = false;

    if(conflag == false)
    {
        ui->pushButton->setText("close");
        my_command_->send_consucf();
    }
    else
    {
        ui->pushButton->setText("connect");
        my_command_->send_bkcon();
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

    if(ui->Position_mode_radioButton->isChecked())
    {
        if(MotorMode != PositionMode){
            MotorMode = PositionMode;
            my_command_->send_motor_mode_choose(Motor_Num,MotorMode);
        }
        my_command_->send_position_send(Motor_Num,ui->Position_expect_lineEdit->text().toFloat());
        ui->Stop_pushButton->setText("STOP");
        stop_flag = false;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning:"),tr("please chose position mode first."));
        //error: please chose position mode first.
    }
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

        if(MotorMode != VelocityMode){
            MotorMode = VelocityMode;
            my_command_->send_motor_mode_choose(Motor_Num,MotorMode);
        }
        my_command_->send_velocity_send(Motor_Num,ui->Velocity_expect_lineEdit->text().toFloat());
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

        if(MotorMode != CurrentMode){
            MotorMode = CurrentMode;
            my_command_->send_motor_mode_choose(Motor_Num,MotorMode);
        }
        my_command_->send_current_send(Motor_Num,ui->Current_expect_lineEdit->text().toFloat());
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
    short Motor_Num = getMotorNum();
    if(stop_flag == false)
    {
        ui->Stop_pushButton->setText("RUN");
        my_command_->send_motor_stop(Motor_Num);
        stop_flag = true;
    }
    else
    {
        ui->Stop_pushButton->setText("STOP");
        my_command_->send_motor_run(Motor_Num);
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

void MainWindow::on_test_pushButton_released()
{
    my_command_->send_test_x_send(1,ui->Current_expect_lineEdit->text().toFloat());

}

static int image_get_cnt = 1;
static int grp_num = 0;
void MainWindow::show_iamge_from_camera()
{
    //get image and convert to pixmap.
    QImage img;
    img.loadFromData(pDataForSaveImage,2448*2048*4+2048,"bmp");
    QPixmap pixmap = QPixmap::fromImage(img);

    //fit image to label window size.
    int i_width = ui->camer_label->width();
    int i_height = ui->camer_label->height();
    QPixmap fitpixmap = pixmap.scaled(i_width,i_height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    //show image.
    ui->camer_label->setPixmap(fitpixmap);
    ui->camer_label->show();

    //put image addr to circle quene.
    //    m_queue_camera->EnQueue(pDataForSaveImage);

    //GET IMAGE EXPERIMENT,20200417.
    static string str2 = "image/";


    if(image_get_cnt<=200)
    {
        //save text.

        static stringstream x_temp;
        static stringstream theta_temp;
        static stringstream grp_num_str;

        if (image_get_cnt ==1)
        {
            //            stringstream xx;
            //            xx<<position_x;
            //            stringstream tt;
            //            tt<<p_theta;

            //            x_temp.swap(xx);
            ////            x_temp<<position_x;
            //            str2.append(x_temp.str());

            //            str2.append("_");

            //            theta_temp.swap(tt);
            ////            theta_temp<<p_theta;
            //            str2.append(theta_temp.str());

            stringstream grp_num_str_reset;
            grp_num_str_reset<<grp_num;

            grp_num_str.swap(grp_num_str_reset);


            str2.append(grp_num_str.str());


            QDir dir;
            dir.mkdir(str2.c_str());//creat foder.
            str2.append("/");
            str2.append(grp_num_str.str());
                        str2.append("_");
                        str2.append(x_temp.str());
            //            str2.append("_");
            //            str2.append(theta_temp.str());
            str2.append(".txt");
        }

        ofstream outfile(str2.c_str(),ios::app);
        outfile<<image_get_cnt<<", "<<image_get_cnt<<", "<<p_theta<<", "<<dp_theta<<"\n";
        outfile.close();


        //save image:
        string str1 = "image/";//name of foder

        //        str1.append(x_temp.str());
        //        str1.append("_");
        //        str1.append(theta_temp.str());

        str1.append(grp_num_str.str());

        str1.append("/");

        stringstream iamg_num;
        iamg_num<<image_get_cnt;
        str1.append(iamg_num.str());
        str1.append(".bmp");

        img.save(str1.c_str(), "bmp");
    }

    if(image_get_cnt == 200)
    {
        my_command_->send_camera_trigger(0);
        str2 = "image/";
    }
    image_get_cnt++;
}

void MainWindow::on_camera_pushButton_released()
{
    static unsigned char flag = 1;
    my_command_->send_camera_trigger(flag);
    if(flag == 1)
    {
        grp_num++;
        image_get_cnt = 1;
    }
    flag = !flag;
}

void MainWindow::on_save_image_pushButton_released()
{
    unsigned char* p;

    //    cout<<m_queue_camera->QueueLength()<<endl;

    int cnt = m_queue_camera->QueueLength();

    for(int i=0;i<cnt;i++)
    {
        string str1 = "image/image";

        stringstream iamg_num;
        iamg_num<<m_queue_camera->QueueLength();
        str1.append(iamg_num.str());
        str1.append(".bmp");

        m_queue_camera->DeQueue(p);
        QImage img;
        img.loadFromData(p,2448*2048*4+2048,"bmp");
        img.save(str1.c_str(), "bmp");
    }
}


void MainWindow::on_Brake_pushButton_released()
{
    if(brake_flag == false)
    {
        ui->Brake_pushButton->setText("Brake on");
        my_command_->send_brake_control((char)brake_flag);
        brake_flag = true;
    }
    else
    {
        ui->Brake_pushButton->setText("Brake off");
        my_command_->send_brake_control((char)brake_flag);
        brake_flag = false;
    }
}

void MainWindow::on_Magnet_1_pushButton_released()
{
    if(magnet1_flag == false)
    {
        ui->Magnet_1_pushButton->setText("Magnet_1 on");
        my_command_->send_magnet1_control((char)magnet1_flag);
        magnet1_flag = true;



        //GET IMAGE EXPERIMENT,20200417.
        on_camera_pushButton_released();
    }
    else
    {
        ui->Magnet_1_pushButton->setText("Magnet_1 off");
        my_command_->send_magnet1_control((char)magnet1_flag);
        magnet1_flag = false;

    }
}

void MainWindow::on_Magnet_2_pushButton_released()
{
    if(magnet2_flag == false)
    {
        ui->Magnet_2_pushButton->setText("Magnet_2 on");
        my_command_->send_magnet2_control((char)magnet2_flag);
        magnet2_flag = true;
    }
    else
    {
        ui->Magnet_2_pushButton->setText("Magnet_2 off");
        my_command_->send_magnet2_control((char)magnet2_flag);
        magnet2_flag = false;
    }
}




void MainWindow::on_PID_test_send_pushButton_released()
{
    static bool open_flag = false;

    if(open_flag == false)
    {
        ui->PID_test_send_pushButton->setText("stop");
        open_flag = true;

        if(!(ui->Kd_lineEdit->text().isEmpty()) && !(ui->Ki_lineEdit->text().isEmpty()) \
                && !(ui->Kp_lineEdit->text().isEmpty()) && !(ui->PID_test_Xd_lineEdit->text().isEmpty()) \
                && !(ui->PID_test_Td_lineEdit->text().isEmpty()))
        {
            Kp = ui->Kp_lineEdit->text().toFloat();
            Ki = ui->Ki_lineEdit->text().toFloat();
            Kd = ui->Kd_lineEdit->text().toFloat();

            if(MotorMode != VelocityMode){
                MotorMode = VelocityMode;
                my_command_->send_motor_mode_choose(1,VelocityMode);
                usleep(5000);
            }

            Xd_ = ui->PID_test_Xd_lineEdit->text().toFloat()/1000.0;
            float t_d = ui->PID_test_Td_lineEdit->text().toFloat();

            //get plan type.
            if(ui->step_planner_radioButton->isChecked())
            {
                PlanType = STEP_PLANNER;
            }
            if(ui->p_planner_radioButton->isChecked())
            {
                PlanType = S_POSITION_PLANNER;
            }

            if(ui->ZVD_checkBox->isChecked())
            {
                ZVD_flag = true;
            }
            else
            {
                ZVD_flag = false;
            }

            m_basic_para->set_Xd(Xd_);
            m_basic_para->set_Xc(position_x/1000.0);
            m_basic_para->set_Td(t_d);

            m_controlthread->set_para(m_basic_para);

            m_controlthread->start();
        }
        else
        {
            return;
            QMessageBox::warning(this,tr("Warning:"),tr(" please set necessary paraments!"));
        }

    }
    else
    {
        ui->PID_test_send_pushButton->setText("start");
        m_controlthread->stopImmediately();

        open_flag = false;

        return;
    }
}


void MainWindow::on_Limit_Velocity_send_pushButton_released()
{
    limit_velocity = ui->Limit_velocity_lineEdit->text().toFloat();
}

void MainWindow::on_read_next_image_pushButton_released()
{
    CameraWorkFlag = true;
}
