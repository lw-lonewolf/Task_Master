/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *BackGround;
    QStackedWidget *stackedWidget;
    QWidget *Home_Page;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *Add_Task;
    QPushButton *Delete_Task;
    QPushButton *Modify_Task;
    QPushButton *P_Button;
    QPushButton *View_More;
    QWidget *ToDoList_Page;
    QWidget *Calendar_Page;
    QWidget *Pomodoro_Page;
    QLabel *Timer_Label;
    QFrame *Play_Button;
    QWidget *Settings_Page;
    QLineEdit *Focus_T;
    QLineEdit *Short_T;
    QLineEdit *Long_T;
    QPushButton *Light_Button;
    QPushButton *Dark_Button;
    QPushButton *Save_Button;
    QPushButton *Reset_Button;
    QWidget *Add_Task_Page;
    QPushButton *Create_Btn;
    QLineEdit *Name_TB;
    QLineEdit *Due_Date;
    QPlainTextEdit *Note_TB;
    QLabel *label_8;
    QLabel *ERR_MSG;
    QWidget *Delete_Task_Page;
    QWidget *Modify_Task_Page;
    QFrame *TaskBar;
    QPushButton *Home_btn;
    QPushButton *ToDo_List_btn;
    QPushButton *Calendar_btn;
    QPushButton *Pomodoro_btn;
    QPushButton *Settings_btn;
    QPushButton *LogOut_btn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1920, 1080);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        BackGround = new QWidget(centralwidget);
        BackGround->setObjectName("BackGround");
        BackGround->setGeometry(QRect(0, 0, 1440, 901));
        sizePolicy.setHeightForWidth(BackGround->sizePolicy().hasHeightForWidth());
        BackGround->setSizePolicy(sizePolicy);
        BackGround->setStyleSheet(QString::fromUtf8("background-image: url(:/Background_Images/New Task_Light.png);"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 10, 1440, 900));
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        Home_Page = new QWidget();
        Home_Page->setObjectName("Home_Page");
        sizePolicy.setHeightForWidth(Home_Page->sizePolicy().hasHeightForWidth());
        Home_Page->setSizePolicy(sizePolicy);
        Home_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        label = new QLabel(Home_Page);
        label->setObjectName("label");
        label->setGeometry(QRect(327, 470, 80, 50));
        QFont font;
        font.setFamilies({QString::fromUtf8("Syne")});
        font.setBold(false);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color:black;\n"
"font-size:20px;"));
        label_2 = new QLabel(Home_Page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(328, 535, 80, 50));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("color:black;\n"
"font-size:20px;"));
        label_3 = new QLabel(Home_Page);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(329, 602, 80, 50));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("color:black;\n"
"font-size:20px;"));
        label_4 = new QLabel(Home_Page);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(330, 665, 80, 50));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("color:black;\n"
"font-size:20px;"));
        label_5 = new QLabel(Home_Page);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(310, 260, 121, 41));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Syne")});
        font1.setPointSize(32);
        font1.setBold(false);
        font1.setItalic(false);
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("color:black;"));
        label_6 = new QLabel(Home_Page);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(625, 265, 91, 30));
        label_6->setFont(font1);
        label_6->setStyleSheet(QString::fromUtf8("color:black;"));
        label_7 = new QLabel(Home_Page);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(388, 92, 191, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Syne")});
        font2.setPointSize(24);
        font2.setBold(false);
        font2.setItalic(false);
        label_7->setFont(font2);
        label_7->setStyleSheet(QString::fromUtf8("color:black;"));
        Add_Task = new QPushButton(Home_Page);
        Add_Task->setObjectName("Add_Task");
        Add_Task->setGeometry(QRect(959, 52, 440, 96));
        Add_Task->setStyleSheet(QString::fromUtf8(" background-color: transparent;\n"
"    border: none;"));
        Add_Task->setFlat(true);
        Delete_Task = new QPushButton(Home_Page);
        Delete_Task->setObjectName("Delete_Task");
        Delete_Task->setGeometry(QRect(957, 163, 440, 96));
        Delete_Task->setStyleSheet(QString::fromUtf8(" background-color: transparent;\n"
"    border: none;"));
        Delete_Task->setFlat(true);
        Modify_Task = new QPushButton(Home_Page);
        Modify_Task->setObjectName("Modify_Task");
        Modify_Task->setGeometry(QRect(956, 270, 440, 96));
        Modify_Task->setStyleSheet(QString::fromUtf8(" background-color: transparent;\n"
"    border: none;"));
        Modify_Task->setFlat(true);
        P_Button = new QPushButton(Home_Page);
        P_Button->setObjectName("P_Button");
        P_Button->setGeometry(QRect(1148, 706, 50, 52));
        P_Button->setStyleSheet(QString::fromUtf8(" background-color: transparent;\n"
"    border: none;"));
        P_Button->setFlat(true);
        View_More = new QPushButton(Home_Page);
        View_More->setObjectName("View_More");
        View_More->setGeometry(QRect(441, 729, 264, 46));
        View_More->setStyleSheet(QString::fromUtf8(" background-color: transparent;\n"
"    border: none;"));
        View_More->setFlat(true);
        stackedWidget->addWidget(Home_Page);
        label_5->raise();
        label_6->raise();
        label_3->raise();
        label_2->raise();
        label_7->raise();
        label_4->raise();
        label->raise();
        Add_Task->raise();
        Delete_Task->raise();
        Modify_Task->raise();
        P_Button->raise();
        View_More->raise();
        ToDoList_Page = new QWidget();
        ToDoList_Page->setObjectName("ToDoList_Page");
        sizePolicy.setHeightForWidth(ToDoList_Page->sizePolicy().hasHeightForWidth());
        ToDoList_Page->setSizePolicy(sizePolicy);
        ToDoList_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        stackedWidget->addWidget(ToDoList_Page);
        Calendar_Page = new QWidget();
        Calendar_Page->setObjectName("Calendar_Page");
        Calendar_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        stackedWidget->addWidget(Calendar_Page);
        Pomodoro_Page = new QWidget();
        Pomodoro_Page->setObjectName("Pomodoro_Page");
        sizePolicy.setHeightForWidth(Pomodoro_Page->sizePolicy().hasHeightForWidth());
        Pomodoro_Page->setSizePolicy(sizePolicy);
        Pomodoro_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        Timer_Label = new QLabel(Pomodoro_Page);
        Timer_Label->setObjectName("Timer_Label");
        Timer_Label->setGeometry(QRect(751, 327, 170, 65));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Syne")});
        font3.setPointSize(22);
        Timer_Label->setFont(font3);
        Timer_Label->setStyleSheet(QString::fromUtf8(""));
        Timer_Label->setAlignment(Qt::AlignCenter);
        Play_Button = new QFrame(Pomodoro_Page);
        Play_Button->setObjectName("Play_Button");
        Play_Button->setGeometry(QRect(790, 686, 87, 90));
        Play_Button->setStyleSheet(QString::fromUtf8("border:transparent;"));
        Play_Button->setFrameShape(QFrame::StyledPanel);
        Play_Button->setFrameShadow(QFrame::Raised);
        stackedWidget->addWidget(Pomodoro_Page);
        Settings_Page = new QWidget();
        Settings_Page->setObjectName("Settings_Page");
        sizePolicy.setHeightForWidth(Settings_Page->sizePolicy().hasHeightForWidth());
        Settings_Page->setSizePolicy(sizePolicy);
        Settings_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        Focus_T = new QLineEdit(Settings_Page);
        Focus_T->setObjectName("Focus_T");
        Focus_T->setGeometry(QRect(959, 315, 350, 43));
        Focus_T->setStyleSheet(QString::fromUtf8("border:transparent;\n"
"background:transparent;"));
        Short_T = new QLineEdit(Settings_Page);
        Short_T->setObjectName("Short_T");
        Short_T->setGeometry(QRect(959, 432, 350, 43));
        Short_T->setStyleSheet(QString::fromUtf8("border:transparent;\n"
"background:transparent;a"));
        Long_T = new QLineEdit(Settings_Page);
        Long_T->setObjectName("Long_T");
        Long_T->setGeometry(QRect(959, 554, 350, 43));
        Long_T->setStyleSheet(QString::fromUtf8("border:transparent;\n"
"background:transparent;"));
        Light_Button = new QPushButton(Settings_Page);
        Light_Button->setObjectName("Light_Button");
        Light_Button->setGeometry(QRect(938, 194, 179, 36));
        Light_Button->setCursor(QCursor(Qt::PointingHandCursor));
        Light_Button->setFlat(true);
        Dark_Button = new QPushButton(Settings_Page);
        Dark_Button->setObjectName("Dark_Button");
        Dark_Button->setGeometry(QRect(1152, 192, 180, 39));
        Dark_Button->setCursor(QCursor(Qt::PointingHandCursor));
        Dark_Button->setFlat(true);
        Save_Button = new QPushButton(Settings_Page);
        Save_Button->setObjectName("Save_Button");
        Save_Button->setGeometry(QRect(330, 700, 196, 47));
        Save_Button->setCursor(QCursor(Qt::PointingHandCursor));
        Save_Button->setFlat(true);
        Reset_Button = new QPushButton(Settings_Page);
        Reset_Button->setObjectName("Reset_Button");
        Reset_Button->setGeometry(QRect(550, 710, 61, 31));
        Reset_Button->setCursor(QCursor(Qt::PointingHandCursor));
        Reset_Button->setFlat(true);
        stackedWidget->addWidget(Settings_Page);
        Add_Task_Page = new QWidget();
        Add_Task_Page->setObjectName("Add_Task_Page");
        Add_Task_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        Create_Btn = new QPushButton(Add_Task_Page);
        Create_Btn->setObjectName("Create_Btn");
        Create_Btn->setGeometry(QRect(704, 683, 191, 45));
        Create_Btn->setStyleSheet(QString::fromUtf8(" background-color: transparent;\n"
" border: none;"));
        Create_Btn->setFlat(true);
        Name_TB = new QLineEdit(Add_Task_Page);
        Name_TB->setObjectName("Name_TB");
        Name_TB->setGeometry(QRect(310, 170, 290, 40));
        Name_TB->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border: 0 transparent;"));
        Due_Date = new QLineEdit(Add_Task_Page);
        Due_Date->setObjectName("Due_Date");
        Due_Date->setGeometry(QRect(1060, 170, 290, 40));
        Due_Date->setStyleSheet(QString::fromUtf8("\n"
"border: 0 transparent;"));
        Due_Date->setClearButtonEnabled(false);
        Note_TB = new QPlainTextEdit(Add_Task_Page);
        Note_TB->setObjectName("Note_TB");
        Note_TB->setGeometry(QRect(317, 310, 1030, 210));
        Note_TB->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border: 0 transparent;"));
        label_8 = new QLabel(Add_Task_Page);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(1070, 220, 160, 17));
        label_8->setStyleSheet(QString::fromUtf8("color:black;"));
        ERR_MSG = new QLabel(Add_Task_Page);
        ERR_MSG->setObjectName("ERR_MSG");
        ERR_MSG->setGeometry(QRect(320, 780, 390, 17));
        ERR_MSG->setStyleSheet(QString::fromUtf8("color:transparent;\n"
"background-color:transparent;\n"
""));
        stackedWidget->addWidget(Add_Task_Page);
        Delete_Task_Page = new QWidget();
        Delete_Task_Page->setObjectName("Delete_Task_Page");
        Delete_Task_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        stackedWidget->addWidget(Delete_Task_Page);
        Modify_Task_Page = new QWidget();
        Modify_Task_Page->setObjectName("Modify_Task_Page");
        Modify_Task_Page->setStyleSheet(QString::fromUtf8("background:transparent;"));
        stackedWidget->addWidget(Modify_Task_Page);
        TaskBar = new QFrame(centralwidget);
        TaskBar->setObjectName("TaskBar");
        TaskBar->setGeometry(QRect(22, 23, 222, 855));
        TaskBar->setStyleSheet(QString::fromUtf8("background:transparent; \n"
"border: transparent;"));
        TaskBar->setFrameShape(QFrame::StyledPanel);
        TaskBar->setFrameShadow(QFrame::Raised);
        Home_btn = new QPushButton(TaskBar);
        Home_btn->setObjectName("Home_btn");
        Home_btn->setGeometry(QRect(42, 182, 90, 30));
        Home_btn->setCursor(QCursor(Qt::PointingHandCursor));
        Home_btn->setFlat(true);
        ToDo_List_btn = new QPushButton(TaskBar);
        ToDo_List_btn->setObjectName("ToDo_List_btn");
        ToDo_List_btn->setGeometry(QRect(40, 260, 118, 30));
        ToDo_List_btn->setCursor(QCursor(Qt::PointingHandCursor));
        ToDo_List_btn->setFlat(true);
        Calendar_btn = new QPushButton(TaskBar);
        Calendar_btn->setObjectName("Calendar_btn");
        Calendar_btn->setGeometry(QRect(40, 350, 110, 29));
        Calendar_btn->setCursor(QCursor(Qt::PointingHandCursor));
        Calendar_btn->setFlat(true);
        Pomodoro_btn = new QPushButton(TaskBar);
        Pomodoro_btn->setObjectName("Pomodoro_btn");
        Pomodoro_btn->setGeometry(QRect(40, 426, 117, 32));
        Pomodoro_btn->setCursor(QCursor(Qt::PointingHandCursor));
        Pomodoro_btn->setFlat(true);
        Settings_btn = new QPushButton(TaskBar);
        Settings_btn->setObjectName("Settings_btn");
        Settings_btn->setGeometry(QRect(40, 505, 102, 34));
        Settings_btn->setCursor(QCursor(Qt::PointingHandCursor));
        Settings_btn->setFlat(true);
        LogOut_btn = new QPushButton(TaskBar);
        LogOut_btn->setObjectName("LogOut_btn");
        LogOut_btn->setGeometry(QRect(40, 780, 90, 34));
        LogOut_btn->setCursor(QCursor(Qt::PointingHandCursor));
        LogOut_btn->setFlat(true);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "No Tasks", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "No Tasks", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "No Tasks", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "No Tasks", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "LoneWolf", nullptr));
        Add_Task->setText(QString());
        Delete_Task->setText(QString());
        Modify_Task->setText(QString());
        P_Button->setText(QString());
        View_More->setText(QString());
        Timer_Label->setText(QCoreApplication::translate("MainWindow", "TIMER HERE ", nullptr));
        Light_Button->setText(QString());
        Dark_Button->setText(QString());
        Save_Button->setText(QString());
        Reset_Button->setText(QString());
        Create_Btn->setText(QString());
        Due_Date->setPlaceholderText(QString());
        label_8->setText(QCoreApplication::translate("MainWindow", "Format : DD/MM/YYYY", nullptr));
        ERR_MSG->setText(QCoreApplication::translate("MainWindow", "INCORRECT FORMAT USED FOR DATE", nullptr));
        Home_btn->setText(QString());
        ToDo_List_btn->setText(QString());
        Calendar_btn->setText(QString());
        Pomodoro_btn->setText(QString());
        Settings_btn->setText(QString());
        LogOut_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
