#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// variable to store either light or dark state in user preference. Light mode by default
QString backgroundState = "Light";

// Sidebar background changes. Setting light mode picture for now. A bool needs to be created that stores the current light/dark state and changes background  accoridngly.
void MainWindow::on_Home_btn_clicked()
{

    if(ui->stackedWidget->currentWidget() != ui->Home_Page) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Home_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Home_Page);
    }
}

void MainWindow::on_ToDo_List_btn_clicked()
{
    if(ui->stackedWidget->currentWidget() != ui->ToDoList_Page) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/To-Do List_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->ToDoList_Page);
    }
}


void MainWindow::on_Calendar_btn_clicked()
{
    if(ui->stackedWidget->currentWidget() != ui->Calendar_Page) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Calendar_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Calendar_Page);
    }
}


void MainWindow::on_Pomodoro_btn_clicked()
{
    if(ui->stackedWidget->currentWidget() != ui->Pomodoro_Page) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Pomodoro_Page);
    }
}


void MainWindow::on_Settings_btn_clicked()
{
    if(ui->stackedWidget->currentWidget() != ui->Settings_Page) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Settings_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    }
}

// Settings
void MainWindow::on_Light_Button_clicked()
{
    ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Settings_Light.png);");
}


void MainWindow::on_Dark_Button_clicked()
{
    ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Settings_Dark.png);");
}


void MainWindow::on_Save_Button_clicked()
{
    if (ui->BackGround->styleSheet() == "background-image: url(:/Background_Images/Settings_Light.png);") {
        backgroundState = "Light";
    }
    else
        backgroundState = "Dark";
}


void MainWindow::on_Reset_Button_clicked()
{
    ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Settings_" + backgroundState + ".png);");
    backgroundState = "Light";
}

