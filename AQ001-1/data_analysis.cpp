#include "data_analysis.h"
#include "ui_data_analysis.h"
#include "mainwindow.h"

#include <QPainter>
#include <QPointF>
#include <QPen>
#include <qmath.h>
#include <iostream>.h>

//#include <sys/time.h>
//struct timeval tpstart,tpend;
//double timeuse;


using namespace std;

void Data_Analysis::UpdateData()
{
    x += timeout_/1000.0/x_scal;
    m_count += 1;

    m_queue_x->EnQueue(position_x);
    m_queue_dx->EnQueue(d_position_x);
    m_queue_theta->EnQueue(theta);
    m_queue_dtheta->EnQueue(d_theta);

    m_queue_t->EnQueue(x);

    int get_cnt = (int)(20.0/(timeout_/1000.0));
    xList = m_queue_t->QueueTraverse(get_cnt);

    switch (ch1_name) {
    case 0x01:
        yList_1 = m_queue_x->QueueTraverse(get_cnt);
        y1_max = y1_max>fabs(position_x) ? y1_max : fabs(position_x);//get max y value.
        break;
    case 0x02:
        yList_1 = m_queue_dx->QueueTraverse(get_cnt);
        y1_max = y1_max>fabs(d_position_x) ? y1_max : fabs(d_position_x);//get max y value.
        break;
    case 0x03:
        yList_1 = m_queue_theta->QueueTraverse(get_cnt);
        y1_max = y1_max>fabs(theta) ? y1_max : fabs(theta);//get max y value.
        break;
    case 0x04:
        yList_1 = m_queue_dtheta->QueueTraverse(get_cnt);
        y1_max = y1_max>fabs(d_theta) ? y1_max : fabs(d_theta);//get max y value.
        break;

    default:
        break;
    }

    switch (ch2_name) {
    case 0x01:
        yList_2 = m_queue_x->QueueTraverse(get_cnt);
        y2_max = y2_max>fabs(position_x) ? y2_max : fabs(position_x);//get max y value.
        break;
    case 0x02:
        yList_2 = m_queue_dx->QueueTraverse(get_cnt);
        y2_max = y2_max>fabs(d_position_x) ? y2_max : fabs(d_position_x);//get max y value.
        break;
    case 0x03:
        yList_2 = m_queue_theta->QueueTraverse(get_cnt);
        y2_max = y2_max>fabs(theta) ? y2_max : fabs(theta);//get max y value.
        break;
    case 0x04:
        yList_2 = m_queue_dtheta->QueueTraverse(get_cnt);
        y2_max = y2_max>fabs(d_theta) ? y2_max : fabs(d_theta);//get max y value.
        break;

    default:
        break;
    }

    update();

    if(y1_max<10)
    {
        y1_max = 10;
    }
    y1_scal = y1_max/20;

    if(y2_max<10)
    {
        y2_max = 10;
    }
    y2_scal = y2_max/20;

    //    y_scal = 1;

}
void Data_Analysis::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //paint view
    painter.setViewport(20, 20, width()-40, height()-90);
    painter.setWindow(-5, -23, 210, 46); // (-10, 2)    (10, -2)
    painter.fillRect(-5, -23, 210, 46, Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点

    QPen pen_ref;
    QPen pen_red;
    QPen pen_blue;

    pen_ref.setColor(Qt::black);
    pen_ref.setStyle(Qt::DashLine);
    pen_ref.setWidthF(0.03);
    painter.setPen(pen_ref);

    // paint reference lines
    painter.drawLine(QPointF(0, 0), QPointF(200, 0));   // y=0
    painter.drawLine(QPointF(100, 20), QPointF(100, -20));     // x=0

    pen_ref.setStyle(Qt::SolidLine);
    pen_ref.setWidthF(0.02);
    painter.setPen(pen_ref);
    painter.drawLine(QPointF(0, 20), QPointF(200, 20));     // x
    painter.drawLine(QPointF(0, -20), QPointF(0, 20));     // y1
    painter.drawLine(QPointF(200, -20), QPointF(200, 20));     // y1

    pen_red.setColor(Qt::red);
    pen_red.setStyle(Qt::SolidLine);
    pen_red.setWidthF(0.2);
    painter.setPen(pen_red);

    //paint data points.
    for(int i = 0; i < yList_1.count(); i++)
    {
        if(i == 0)
        {
            painter.drawPoint(QPointF(xList[i]-xList[0], -yList_1[i]/y1_scal));
        }

        else
        {
            painter.drawLine(QPointF(xList[i-1]-xList[0], -yList_1[i-1]/y1_scal), QPointF(xList[i]-xList[0], -yList_1[i]/y1_scal));
        }
    }

    pen_red.setColor(Qt::blue);
    pen_red.setStyle(Qt::SolidLine);
    pen_red.setWidthF(0.2);
    painter.setPen(pen_red);

    for(int i = 0; i < yList_1.count(); i++)
    {
        if(i == 0)
        {
            painter.drawPoint(QPointF(xList[i]-xList[0], -yList_2[i]/y2_scal));
        }

        else
        {
            painter.drawLine(QPointF(xList[i-1]-xList[0], -yList_2[i-1]/y2_scal), QPointF(xList[i]-xList[0], -yList_2[i]/y2_scal));
        }
    }


    //绘制刻度线
    QPen pen_axis;
    pen_axis.setColor(Qt::black);
    pen_axis.setWidth(0.02);
    painter.setPen(pen_axis);

    QFont font1("Tahoma",2,QFont::Normal,false);
    painter.setFont(font1);

    float x_offset = 0;

    //画上xy轴刻度线
    for(int i=0;i<10;i++)//分成10份
    {
        //选取合适的坐标，绘制一段长度为4的直线，用于表示刻度
        painter.drawLine(QPointF(0+(i+(1-x_offset))*20,20),QPointF(0+(i+(1-x_offset))*20,21));//x
        painter.drawLine(QPointF(0,-20+(i)*4),QPointF(0-1,-20+(i)*4));//y1
        painter.drawLine(QPointF(200,-20+(i)*4),QPointF(200+1,-20+(i)*4));//y2


        painter.drawText(QPointF(0+(i+0.9-x_offset)*20,19),QString::number((int)((i+1)*2)));//x data

        painter.drawText(2,-19+i*4,QString::number(((20-i*4)*y1_scal),'f',2));//y1 data
        painter.drawText(190,-19+i*4,QString::number(((20-i*4)*y2_scal),'f',2));//y1 data
    }

    painter.drawText(100,23,"Time(s)");
    painter.drawText(-5,0,"y1");
    painter.drawText(202,0,"y2");

    //**debug, check timeout.**//
    static int cnt_timeout = 0;
    if(cnt_timeout%1000==0)
    {
        //        gettimeofday(&tpend,NULL);
        //        timeuse = (1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec)/1000000.0;
        //        cout << "time use:"<<timeuse <<"s."<<"  cnt:"<<cnt_timeout<<endl;
        //        gettimeofday(&tpstart,NULL);
    }
    cnt_timeout++;

}

Data_Analysis::Data_Analysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Data_Analysis)
{
    ui->setupUi(this);
    pointx=35;
    pointy=280;//确定坐标轴起点坐标，这里定义(35,280)

    ch1_name = 0x01;
    ch2_name = 0x02;
    timeout_ = 20;
    y1_scal = 1;
    y2_scal = 1;

    x_scal = 0.1;
    x = 0;
    m_count = 0;
    m_Timer = new QTimer(this);
    m_Timer->start(timeout_);
    connect(m_Timer,SIGNAL(timeout()),this,SLOT(UpdateData()));
    ui->setupUi(this);

    //    m_queue_t(1000);
    m_queue_t = new Circle_Queue<float>(20000);
    m_queue_x = new Circle_Queue<float>(20000);
    m_queue_dx = new Circle_Queue<float>(20000);
    m_queue_theta = new Circle_Queue<float>(20000);
    m_queue_dtheta = new Circle_Queue<float>(20000);

    connect(ui->stop_pushButton,SIGNAL(released()),this,SLOT(on_stop_pushButton_released()));
    connect(ui->plot_reset,SIGNAL(released()),this,SLOT(on_plot_reset_released()));
    connect(ui->ch1_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_ch1_comboBox_currentIndexChanged(int)));
    connect(ui->ch2_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_ch2_comboBox_currentIndexChanged(int)));

}

Data_Analysis::~Data_Analysis()
{
    delete ui;
}

void Data_Analysis::on_plot_reset_released()
{
    y1_max = 10;
    y2_max = 10;
    m_queue_t->ClearQueue();
    m_queue_x->ClearQueue();
    m_queue_dx->ClearQueue();
    m_queue_theta->ClearQueue();
    m_queue_dtheta->ClearQueue();

}


void Data_Analysis::on_ch1_comboBox_currentIndexChanged(int index)
{

    switch (index) {
    case 0x00:
        ch1_name = X_DATA;
        break;
    case 0x01:
        ch1_name = DX_DATA;
        break;
    case 0x02:
        ch1_name = THETA_DATA;
        break;
    case 0x03:
        ch1_name = DTHETA_DATA;
        break;


    default:
        break;
    }

}

void Data_Analysis::on_ch2_comboBox_currentIndexChanged(int index)
{

    switch (index) {
    case 0x00:
        ch2_name = X_DATA;
        break;
    case 0x01:
        ch2_name = DX_DATA;
        break;
    case 0x02:
        ch2_name = THETA_DATA;
        break;
    case 0x03:
        ch2_name = DTHETA_DATA;
        break;


    default:
        break;
    }

}

void Data_Analysis::on_stop_pushButton_released()
{
    static bool plot_flag = true;

    if(plot_flag == false)
    {
        on_plot_reset_released();
        ui->stop_pushButton->setText("stop");
        plot_flag = true;
        m_Timer->start();
    }
    else
    {
        m_Timer->stop();

        ui->stop_pushButton->setText("plot");
        plot_flag = false;
    }
}
