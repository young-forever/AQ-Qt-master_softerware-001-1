#ifndef DATA_ANALYSIS_H
#define DATA_ANALYSIS_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include "circle_queue.h"

#define X_DATA 0X01;
#define DX_DATA 0X02;
#define THETA_DATA 0X03;
#define DTHETA_DATA 0X04;


namespace Ui {
class Data_Analysis;
}

class Data_Analysis : public QWidget
{
    Q_OBJECT

    char ch1_name;
    char ch2_name;


    int pointx;
    int pointy;//确定坐标轴起点坐标，这里定义(35,280)
    int timeout_;//
    float y1_scal;
    float y2_scal;
    float x_scal;
    float y1_max;
    float y2_max;
    int m_count;
    float x;

    Circle_Queue<float>* m_queue_t;

    Circle_Queue<float>* m_queue_x;
    Circle_Queue<float>* m_queue_dx;
    Circle_Queue<float>* m_queue_theta;
    Circle_Queue<float>* m_queue_dtheta;


    QTimer* m_Timer;
    QList<float> xList;
    QList<float> yList_1;
    QList<float> yList_2;


protected:
    void paintEvent(QPaintEvent *);

public slots:
    void UpdateData();

public:
    explicit Data_Analysis(QWidget *parent = 0);
    ~Data_Analysis();

private slots:
    void on_plot_reset_released();

    void on_ch1_comboBox_currentIndexChanged(int index);

    void on_ch2_comboBox_currentIndexChanged(int index);

    void on_stop_pushButton_released();

private:
    Ui::Data_Analysis *ui;


};

#endif // DATA_ANALYSIS_H
