#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
//-- DB Management --//
#include "dbmanager.h"

//-- Included for Locating Children --//
#include <QDebug>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
//-- Date Functionality --//
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }
// variable to store either light or dark state in user preference. Light mode
// by default
QString backgroundState = "Light";

// Task Manager Initialization
QString user_path = "/home/lonewolf/Test_Run/tasks.db";
DbManager Task_Manager(user_path);

// Sidebar background changes. Setting light mode picture for now. A bool needs
// to be created that stores the current light/dark state and changes background
// accoridngly.
void MainWindow::on_Home_btn_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Home_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Home_" + backgroundState +
        ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Home_Page);
  }
}

void MainWindow::on_ToDo_List_btn_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->ToDoList_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/To-Do List_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->ToDoList_Page);
  }
}

void MainWindow::on_Calendar_btn_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Calendar_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Calendar_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Calendar_Page);
  }
}

void MainWindow::on_Pomodoro_btn_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Pomodoro_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Pomodoro_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Pomodoro_Page);
  }
}

void MainWindow::on_Settings_btn_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Settings_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Settings_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
  }
}

// Settings
void MainWindow::on_Light_Button_clicked() {
  ui->BackGround->setStyleSheet(
      "background-image: url(:/Background_Images/Settings_Light.png);");
}

void MainWindow::on_Dark_Button_clicked() {
  ui->BackGround->setStyleSheet(
      "background-image: url(:/Background_Images/Settings_Dark.png);");
}

void MainWindow::on_Save_Button_clicked() {
  if (ui->BackGround->styleSheet() ==
      "background-image: url(:/Background_Images/Settings_Light.png);") {
    backgroundState = "Light";
  } else
    backgroundState = "Dark";
}

void MainWindow::on_Reset_Button_clicked() {
  ui->BackGround->setStyleSheet(
      "background-image: url(:/Background_Images/Settings_" + backgroundState +
      ".png);");
  backgroundState = "Light";
}

void MainWindow::on_View_More_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->ToDoList_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/To-Do List_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->ToDoList_Page);
  }
}

void MainWindow::on_P_Button_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Pomodoro_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Pomodoro_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Pomodoro_Page);
  }
}

void MainWindow::on_Add_Task_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Add_Task_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/New Task_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Add_Task_Page);
  }
}

void MainWindow::on_Delete_Task_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Delete_Task_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Delete Task_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Delete_Task_Page);
  }
}

void MainWindow::on_Modify_Task_clicked() {
  if (ui->stackedWidget->currentWidget() != ui->Modify_Task_Page) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Modify Task_" +
        backgroundState + ".png);");
    ui->stackedWidget->setCurrentWidget(ui->Modify_Task_Page);
  }
}

void MainWindow::on_Create_Btn_clicked() {
  Task_Manager.createTasksTable();
  QDate currentDate = QDate::currentDate();

  // Variables for Name, Date, and Comments
  QString NAME = ui->Name_TB->text();
  QString DATE = ui->Due_Date->text();
  QString NOTES = ui->Note_TB->toPlainText();

  // Due Date Seperation
  unsigned short day = ((DATE[0].digitValue()) * 10) + DATE[1].digitValue();
  unsigned short month = ((DATE[3].digitValue() * 10) + DATE[4].digitValue());
  unsigned short year =
      ((DATE[6].digitValue() * 1000) + (DATE[7].digitValue() * 100) +
       (DATE[8].digitValue() * 10) + DATE[9].digitValue());

  // Due Date Variable Creation
  int d_date = 0;
  if (day >= 10) {
    d_date += (day * 1000000) + (month * 10000) + year;
  } else {
    d_date += (day * 10000000) + (month * 10000) + year;
  }

  // Current Date Variable Creation
  int c_date = 0;
  if (currentDate.day() >= 10) {
    c_date += (currentDate.day() * 1000000) + (currentDate.month() * 10000) +
              currentDate.year();
  } else {
    c_date += (currentDate.day() * 10000000) + (currentDate.month() * 10000) +
              currentDate.year();
  }

  if (DATE.length() != 10) {
    ui->ERR_MSG->setText("Correct Format not Followed. Please try again.");
    ui->ERR_MSG->setStyleSheet("color: red;");
  } else if (day > 30 || month < 1 || month > 12 || year < currentDate.year() ||
             year > currentDate.year() + 2) {
    ui->ERR_MSG->setText("Incorrect Date entered. Please try again.");
    ui->ERR_MSG->setStyleSheet("color: red;");
  } else {
    if (Task_Manager.taskExists(NAME, c_date, d_date)) {
      ui->ERR_MSG->setText(
          "Another task with the same name and dates already exists.");
      ui->ERR_MSG->setStyleSheet("color: red;");
    } else {
      if (Task_Manager.addTask(NAME, c_date, d_date, NOTES)) {
        ui->ERR_MSG->setText("Operation Completed successfully.");
        ui->ERR_MSG->setStyleSheet("color:green;");
      } else {
        ui->ERR_MSG->setText("Operation Failed.");
        ui->ERR_MSG->setStyleSheet("color:red;");
      }
    }
  }
}
