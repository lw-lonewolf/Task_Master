#include "mainwindow.h"
#include "ui_mainwindow.h"
// Database
//-- Included for Locating Children --//
#include <QDebug>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
#include <QVBoxLayout>
//-- Date Functionality --//
#include <QDate>

MainWindow::MainWindow(DbManager& obj, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    Task_Manager = &obj;
    Task_Manager->createTasksTable();
}

MainWindow::~MainWindow() { delete ui; }

// variable to store either light or dark state in user preference. Light mode
// by default
QString backgroundState = "Light";
// Sidebar background changes. Setting light mode picture for now. A bool needs
// to be created that stores the current light/dark state and changes background
// accoridngly.

void MainWindow::layout_refresh(){
    int count = 0;
    prev_button = nullptr;
    Task_Manager->CountTotalTasks(count);
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    ui->scrollArea->setWidget(widget);
    int buttonW = 308, buttonH = 20;

    widget->setMinimumSize(buttonW, count*(buttonH+20));
    for(int i = 0; i < count; i++){
        QPushButton *button = new QPushButton(Task_Manager->getName(i));
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, i, button]() {handleButtonClicked(i,button);} );
        button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #F1F1F1; min-width: 308px; min-height:20px; margin-bottom: 20px;");
        if (i == current_selected_button) {
            button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #FFDC81 ; min-width: 308px; min-height:20px; margin-bottom:20px;");
        }
    }
    layout->insertStretch(-1,1);
}

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

// Home
void MainWindow::on_Add_Task_clicked() {
    Task_Manager->createTasksTable();

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
    layout_refresh();
}


// Add Task
void MainWindow::on_Create_Btn_clicked() {
    if(ui->Name_TB->text() == "" || ui->Due_Date->text() == "") {
        ui->ERR_MSG_A_P->setText("Name and Due Date Field cannot be empty!");
        ui->ERR_MSG_A_P->setStyleSheet("color: red;");
        return;
    }
    if(ui->Note_TB->toPlainText().length() > 1024){
        ui->ERR_MSG_A_P->setText("Notes can not be more than 1024 characters.");
        ui->ERR_MSG_A_P->setStyleSheet("color: red;");
        return;
    }
    // Variables for Name, Date, and Comments
    QString NAME = ui->Name_TB->text();
    QString NOTES = ui->Note_TB->toPlainText();
    QDate DATE = QDate::fromString(ui->Due_Date->text(), "dd/MM/yyyy");
    QString c_date_str = QDate::currentDate().toString("dd/MM/yyyy");
    QString d_date_str = DATE.toString("dd/MM/yyyy");

//VALIDATION
    if(DATE.isValid() && DATE > QDate::currentDate()){
        if (Task_Manager->taskExists(NAME, c_date_str, d_date_str)) {
                ui->ERR_MSG_A_P->setText(
                    "Another task with the same name and dates already exists.");
                ui->ERR_MSG_A_P->setStyleSheet("color: red;");
        }
        else {
            if (Task_Manager->addTask(NAME, c_date_str, d_date_str, NOTES)) {
                ui->ERR_MSG_A_P->setText("Operation Completed successfully.");
                ui->ERR_MSG_A_P->setStyleSheet("color:green;");
            }
            else {
                ui->ERR_MSG_A_P->setText("Operation Failed.");
                ui->ERR_MSG_A_P->setStyleSheet("color:red;");
            }
        }
    }
    else{
        ui->ERR_MSG_A_P->setText("Incorrect Date entered. Please try again.");
        ui->ERR_MSG_A_P->setStyleSheet("color: red;");
    }
}


void MainWindow::handleButtonClicked(const int id, QPushButton* button){
    if (prev_button != nullptr) {
        prev_button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #F1F1F1; min-width: 308px; min-height:20px; margin-bottom:20px;");
    }
    prev_button = button;
    button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #FFDC81 ; min-width: 308px; min-height:20px; margin-bottom:20px;");
    ui->name_M_P->setText(Task_Manager->getName(id));
    ui->c_date_M_P->setText(Task_Manager->getCreationDate(id));
    ui->d_date_M_P->setText(Task_Manager->getDueDate(id));
    ui->notes_M_P->setPlainText(Task_Manager->getNotes(id));
    current_selected_button = id;
}

void MainWindow::on_name_M_P_editingFinished()
{
    ui->ERR_MSG_M_P->setText("");
    if(ui->name_M_P->text() == "" || ui->name_M_P->text().length() > 128){
        ui->ERR_MSG_M_P->setText("Name Field Empty or exceeds Character Limit(128).");
        ui->ERR_MSG_M_P->setStyleSheet("color: red;");
    validName = false;
    }
    validName = true;
}


void MainWindow::on_d_date_M_P_editingFinished()
{
    QDate DATE = QDate::fromString(ui->d_date_M_P->text(), "dd/MM/yyyy");
    ui->ERR_MSG_M_P->setText("");
    if(!DATE.isValid() || DATE <= QDate::currentDate() ||  ui->d_date_M_P->text() == ""){
        ui->ERR_MSG_M_P->setText("Date Field Empty or incorrect Date Entered.");
        ui->ERR_MSG_M_P->setStyleSheet("color: red;");
        validDDate = false;
        return;
    }
    validDDate = true;
}



void MainWindow::on_notes_M_P_textChanged()
{
    ui->ERR_MSG_M_P->setText("");
    if(ui->notes_M_P->toPlainText().length() > 1024){
        ui->ERR_MSG_M_P->setText("Notes can not be more than 1024 characters.");
        ui->ERR_MSG_M_P->setStyleSheet("color: red;");
        validNotes = false;
        return;
    }
    validNotes = true;
}


void MainWindow::on_save_btn_M_P_clicked()
{
    if(validDDate && validName && validNotes){
        QString name, c_date, date, notes;
        name = ui->name_M_P->text();
        c_date = ui->c_date_M_P->text();
        date = ui->d_date_M_P->text();
        notes = ui->notes_M_P->toPlainText();
        qDebug() << current_selected_button;
        qDebug() << name;
        qDebug() << date;
        qDebug() << notes;
        if(Task_Manager->taskExists(name,c_date, date, notes)){
            ui->ERR_MSG_M_P->setText("Task Already exists with same Details.");
            ui->ERR_MSG_M_P->setStyleSheet("color: red;");
        }
        else{
            ui->ERR_MSG_M_P->setText("Operation Successful");
            ui->ERR_MSG_M_P->setStyleSheet("color:green;");
            Task_Manager->setName(current_selected_button, name);
            Task_Manager->setDueDate(current_selected_button, date);
            Task_Manager->setNotes(current_selected_button, notes);
            layout_refresh();
        }
    }
}

