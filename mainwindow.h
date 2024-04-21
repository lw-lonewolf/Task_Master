#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "dbmanager.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DbManager& obj, QWidget *parent = nullptr);
    void layout_creation();
    void layout_destruct();
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

    void handleButtonClicked(const int id, QPushButton* button);
    void on_name_M_P_textChanged();
    void on_d_date_M_P_textChanged();
    void on_notes_M_P_textChanged();
    void on_save_btn_M_P_clicked();

private:
    Ui::MainWindow *ui;
    DbManager* Task_Manager;
    QPushButton* prev_button;
    bool validName;
    bool validDDate;
    bool validNotes;
    bool changed;
    int current_selected_button;
};
#endif // MAINWINDOW_H
