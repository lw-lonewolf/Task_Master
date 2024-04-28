#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "dbmanager.h"
#include "qdatetime.h"
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
    void layout_creation(QScrollArea* scrollArea, bool isMPPage);
    void layout_destruct(QScrollArea* scrollArea);
    void layout_TD_P_Today_create(QScrollArea* scrollArea, bool isToday = true);
    void layout_TD_P_destruct(QScrollArea* scrollArea);
    QPushButton* findButtonById(int id, QScrollArea* scrollArea);
    void incrementPomodoroCycle();
    void updatePomodorotime();
    bool backToPomodoro();
    QString intTOString(int num);
    ~MainWindow();    
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
    // void handleModifyButtonClicked(const int id);

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

private:
    Ui::MainWindow *ui;
    DbManager* Task_Manager;
    QPushButton* prev_button;
    QString backgroundState = "Light";
    bool backgroundChanged = false;
    bool validName = false;
    bool validDDate = false;
    bool validNotes = false;
    bool changed = false;
    int current_selected_button;
    int deleteTaskCounter = 0;
    int focusTime = 25;
    int shortBreak = 5;
    int longBreak = 15;
    bool validFocusTime = false;
    bool validShortBreak = false;
    bool validLongBreak = false;
    QTime* time = nullptr;
    QTimer* timer = nullptr;
    int remainingTime;
    int pomodoroCycles = 0;
    bool shortBreakGiven = false;
};
#endif // MAINWINDOW_H
