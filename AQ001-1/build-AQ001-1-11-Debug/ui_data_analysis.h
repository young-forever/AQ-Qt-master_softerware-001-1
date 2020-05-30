/********************************************************************************
** Form generated from reading UI file 'data_analysis.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATA_ANALYSIS_H
#define UI_DATA_ANALYSIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Data_Analysis
{
public:
    QPushButton *plot_reset;
    QComboBox *ch1_comboBox;
    QLabel *label;
    QLabel *label_2;
    QComboBox *ch2_comboBox;
    QPushButton *stop_pushButton;

    void setupUi(QWidget *Data_Analysis)
    {
        if (Data_Analysis->objectName().isEmpty())
            Data_Analysis->setObjectName(QStringLiteral("Data_Analysis"));
        Data_Analysis->resize(900, 400);
        Data_Analysis->setMouseTracking(true);
        plot_reset = new QPushButton(Data_Analysis);
        plot_reset->setObjectName(QStringLiteral("plot_reset"));
        plot_reset->setGeometry(QRect(430, 360, 51, 27));
        ch1_comboBox = new QComboBox(Data_Analysis);
        ch1_comboBox->setObjectName(QStringLiteral("ch1_comboBox"));
        ch1_comboBox->setGeometry(QRect(180, 360, 61, 27));
        label = new QLabel(Data_Analysis);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 370, 31, 17));
        label_2 = new QLabel(Data_Analysis);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(290, 370, 31, 17));
        ch2_comboBox = new QComboBox(Data_Analysis);
        ch2_comboBox->setObjectName(QStringLiteral("ch2_comboBox"));
        ch2_comboBox->setGeometry(QRect(320, 360, 61, 27));
        stop_pushButton = new QPushButton(Data_Analysis);
        stop_pushButton->setObjectName(QStringLiteral("stop_pushButton"));
        stop_pushButton->setGeometry(QRect(538, 360, 61, 27));

        retranslateUi(Data_Analysis);

        QMetaObject::connectSlotsByName(Data_Analysis);
    } // setupUi

    void retranslateUi(QWidget *Data_Analysis)
    {
        Data_Analysis->setWindowTitle(QApplication::translate("Data_Analysis", "Form", 0));
        plot_reset->setText(QApplication::translate("Data_Analysis", "reset", 0));
        ch1_comboBox->clear();
        ch1_comboBox->insertItems(0, QStringList()
         << QApplication::translate("Data_Analysis", "x", 0)
         << QApplication::translate("Data_Analysis", "d_x", 0)
         << QApplication::translate("Data_Analysis", "theta", 0)
         << QApplication::translate("Data_Analysis", "d_theta", 0)
         << QApplication::translate("Data_Analysis", "roll", 0)
         << QApplication::translate("Data_Analysis", "pitch", 0)
         << QApplication::translate("Data_Analysis", "yaw", 0)
        );
        label->setText(QApplication::translate("Data_Analysis", "CH1", 0));
        label_2->setText(QApplication::translate("Data_Analysis", "CH2", 0));
        ch2_comboBox->clear();
        ch2_comboBox->insertItems(0, QStringList()
         << QApplication::translate("Data_Analysis", "x", 0)
         << QApplication::translate("Data_Analysis", "d_x", 0)
         << QApplication::translate("Data_Analysis", "theta", 0)
         << QApplication::translate("Data_Analysis", "d_theta", 0)
         << QApplication::translate("Data_Analysis", "roll", 0)
         << QApplication::translate("Data_Analysis", "pitch", 0)
         << QApplication::translate("Data_Analysis", "yaw", 0)
        );
        stop_pushButton->setText(QApplication::translate("Data_Analysis", "stop", 0));
    } // retranslateUi

};

namespace Ui {
    class Data_Analysis: public Ui_Data_Analysis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATA_ANALYSIS_H
