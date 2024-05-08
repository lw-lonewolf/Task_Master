#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "dbmanager.h"
#include "qdatetime.h"
#include "qlabel.h"
#include "qtimer.h"
#include <qscrollarea.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DbManager& obj, QWidget *parent = nullptr);
    ~MainWindow();

    // Helper functions
    void layout_creation(QScrollArea* scrollArea, bool isMPPage);
    void layout_destruct(QScrollArea* scrollArea);
    void layout_TD_P_Today_create(QScrollArea* scrollArea, bool isToday = true);
    void layout_TD_P_destruct(QScrollArea* scrollArea);
    int layout_Search_create();
    void layout_Search_destroy();
    QPushButton* findButtonById(int id, QScrollArea* scrollArea);
    void incrementPomodoroCycle();
    void updatePomodorotime();
    bool backToPomodoro();
    void implementStatus (const int id, const int user_id, QWidget*& statusWidget, QLabel*& statusText);
    void changeFontColor();
    QString intTOString(int num);  
private slots:
    void on_Home_btn_clicked();
    void on_ToDo_List_btn_clicked();
    void on_Calendar_btn_clicked();
    void on_Pomodoro_btn_clicked();
    void on_Settings_btn_clicked();
    void on_Light_Button_clicked();
    void on_Dark_Button_clicked();
    void on_Save_Button_clicked();
    void on_Reset_Button_clicked();
    void on_View_More_clicked();
    void on_P_Button_clicked();
    void on_Add_Task_clicked();
    void on_Delete_Task_clicked();
    void on_Modify_Task_clicked();

    void on_Create_Btn_clicked();

    void handleButtonClicked(const int id, QPushButton* button, bool isMPPage);
    void handlebuttonClickedWithId(int id, QScrollArea* scrollArea);
    void handleSearchButtonClick(const int id);

    void on_name_M_P_textChanged();
    void on_d_date_M_P_textChanged();
    void on_notes_M_P_textChanged();
    void on_save_btn_M_P_clicked();
    void on_stat_change_M_P_clicked();
    void on_delete_btn_D_P_clicked();
    void on_Focus_duration_textChanged();
    void on_Short_duration_textChanged();
    void on_Long_duration_textChanged();
    void on_Timer_btn_clicked();

    void on_Login_username_textChanged();

    void on_Login_password_textChanged();

    void on_Login_btn_clicked();

    void on_sendToRegister_btn_clicked();

    void on_Register_username_textChanged();

    void on_Register_password_textChanged();

    void on_Register_confirmPassword_textChanged();

    void on_Register_btn_clicked();

    void on_sendToLogin_btn_clicked();

    void on_LogOut_btn_clicked();

    void on_Search_Name_User_textChanged();

    void on_Search_Creation_Date_User_textChanged();

    void on_Search_btn_clicked();

    void on_Search_btn_taskbar_clicked();

    void on_Search_back_btn_clicked();

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_Calendar_prev_btn_clicked();

    void on_Calendar_next_btn_clicked();

private:
    // variables used at startup (MainWindow constructor)
    Ui::MainWindow *ui;
    DbManager* Task_Manager;
    QString backgroundState;
    QString fontcolor;
    bool backgroundChanged;
    int focusTime;
    int shortBreak;
    int longBreak;
    bool validFocusTime;
    bool validShortBreak;
    bool validLongBreak;
    QTime* time;
    QTimer* timer;
    int remainingTime;
    int pomodoroCycles;
    bool BreakGiven;

    // Variables used to store various properties
    QPushButton* prev_button;
    bool validName;
    bool validDDate;
    bool validNotes;
    bool changed;
    int current_selected_button;
    int deleteTaskCounter;
    bool validSearchName;
    bool validSearchDate;

    // Login and SignUp
    int user_id;
    bool validLoginUsername;
    bool validLoginPassword;
    bool validRegisterUsername;
    bool validRegisterPassword;
    bool validRegisterConfirmPassword;
};
#endif // MAINWINDOW_H
