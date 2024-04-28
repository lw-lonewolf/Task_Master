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
#include <string>

MainWindow::MainWindow(DbManager& obj, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    Task_Manager = &obj;
    Task_Manager->createTasksTable();

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePomodorotime);
    time = new QTime(0, focusTime, 0, 0);
    remainingTime = 60 * focusTime;
}

MainWindow::~MainWindow() { delete ui; }

// Helper functions
QString MainWindow::intTOString(int num) {
    QString result;

    while (num != 0) {
        int digit = num % 10;
        char asciiChar = '0' + digit;
        result = asciiChar + result;
        num /= 10;
    }

    return result;
}

void MainWindow::layout_creation(QScrollArea* scrollArea, bool isMPPage){
    int count = 0;
    prev_button = nullptr;
    Task_Manager->CountTotalTasks(count);
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    scrollArea->setWidget(widget);
    int buttonW = 308, buttonH = 20;

    widget->setMinimumSize(buttonW, count*(buttonH+20));
    for(int i = 0; i < count; i++){
        QPushButton *button = new QPushButton(Task_Manager->getName(i));
        button->setObjectName(QString("taskbutton_%1").arg(i));
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, i, button, isMPPage]() {handleButtonClicked(i,button, isMPPage);} );
        button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #F1F1F1; min-width: 308px; min-height:20px; margin-bottom: 20px;");
        if (i == current_selected_button && changed) {
            button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #FFDC81 ; min-width: 308px; min-height:20px; margin-bottom:20px;");
            prev_button = button;
        }
    }
    layout->insertStretch(-1,1);
}


void MainWindow::layout_destruct(QScrollArea* scrollArea){
    QWidget *widget = scrollArea->widget();
    if(widget){
        QLayout *layout = widget->layout();
        if(layout){
            while(QLayoutItem* item = layout->takeAt(0)){
                if(QWidget* childWidget = item->widget()){
                    delete childWidget;
                }
                delete item;
            }
            delete layout;
        }
        delete widget;
        scrollArea->setWidget(nullptr);
    }
}

// SideBar
void MainWindow::on_Home_btn_clicked() {
    Task_Manager->createTasksTable();
    if (ui->stackedWidget->currentWidget() != ui->Home_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Home_" + backgroundState +
            ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Home_Page);
    }
    Task_Manager->sortTasks();
}

void MainWindow::on_ToDo_List_btn_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->ToDoList_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/To-Do List_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->ToDoList_Page);
    }
    Task_Manager->sortTasks();
    layout_TD_P_Today_create(ui->scrollArea_TD_P_T, true);
    layout_TD_P_Today_create(ui->scrollArea_TD_P_UT, false);
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
    if (ui->Timer_Label->text() == "") {
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        ui->Timer_Label->setStyleSheet("color: rgb(0, 0, 0);");
    }
}

void MainWindow::on_Settings_btn_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Settings_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Settings_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    }
    ui->Focus_duration->setText(intTOString(focusTime));
    ui->Focus_duration->setStyleSheet("font: 600 11pt Manrope; color: rgb(0, 0, 0);");

    ui->Short_duration->setText(intTOString(shortBreak));
    ui->Short_duration->setStyleSheet("font: 600 11pt Manrope; color: rgb(0, 0, 0);");

    ui->Long_duration->setText(intTOString(longBreak));
    ui->Long_duration->setStyleSheet("font: 600 11pt Manrope; color: rgb(0, 0, 0);");

}

// Home
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
    layout_creation(ui->scrollArea_D_P, false);
}

void MainWindow::on_Modify_Task_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Modify_Task_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Modify Task_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Modify_Task_Page);
    }
    layout_creation(ui->scrollArea_M_P, true);

}

// Settings
void MainWindow::on_Light_Button_clicked() {
    if (ui->BackGround->styleSheet() != "background-image: url(:/Background_Images/Settings_Light.png);") {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Settings_Light.png);");
        backgroundChanged = true;
    }
    else
        backgroundChanged = false;
}

void MainWindow::on_Dark_Button_clicked() {
    if (ui->BackGround->styleSheet() != "background-image: url(:/Background_Images/Settings_Dark.png);") {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Settings_Dark.png);");
        backgroundChanged = true;
    }
    else
        backgroundChanged = false;
}

void MainWindow::on_Save_Button_clicked() {
    if (focusTime == ui->Focus_duration->text().toInt() && shortBreak == ui->Short_duration->text().toInt() && longBreak == ui->Long_duration->text().toInt() && !backgroundChanged) {
        ui->ERR_MSG_S_P->setText("Already Saved");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        return;
    }
    else if (validFocusTime && validShortBreak && validLongBreak && backgroundChanged) {
        focusTime = ui->Focus_duration->text().toInt();
        shortBreak = ui->Short_duration->text().toInt();
        longBreak = ui->Long_duration->text().toInt();
        ui->ERR_MSG_S_P->setText("Saved sucessfully");
        ui->ERR_MSG_S_P->setStyleSheet("color: green;");

        if (ui->BackGround->styleSheet() ==
            "background-image: url(:/Background_Images/Settings_Light.png);") {
            backgroundState = "Light";
        } else
            backgroundState = "Dark";
        backgroundChanged = false;
    }
    else {
        if (backgroundChanged) {
            if (ui->BackGround->styleSheet() ==
                "background-image: url(:/Background_Images/Settings_Light.png);") {
                backgroundState = "Light";
            } else
                backgroundState = "Dark";
            ui->ERR_MSG_S_P->setText("Color Scheme saved sucessfully");
            ui->ERR_MSG_S_P->setStyleSheet("color: green;");
            backgroundChanged = false;
        }
        else if (!validFocusTime) {
            ui->ERR_MSG_S_P->setText("Focus Duration is invalid. Make sure it is a multiple of 5");
            ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        }
        else if (!validShortBreak) {
            ui->ERR_MSG_S_P->setText("Short Break Duration is invalid. Make sure it is a 5th of the Focus Time.");
            ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        }
        else if (!validLongBreak) {
            ui->ERR_MSG_S_P->setText("Long Break Duration is invalid. Make sure it is 3/5th of the Focus Duration");
            ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        }
        else {
            focusTime = ui->Focus_duration->text().toInt();
            shortBreak = ui->Short_duration->text().toInt();
            longBreak = ui->Long_duration->text().toInt();
            ui->ERR_MSG_S_P->setText("Saved sucessfully");
            ui->ERR_MSG_S_P->setStyleSheet("color: green;");
        }
    }
}

void MainWindow::on_Reset_Button_clicked() {
    backgroundState = "Light";
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Settings_" + backgroundState +
        ".png);");
    ui->Focus_duration->setText("25");
    ui->Short_duration->setText("5");
    ui->Long_duration->setText("15");

    focusTime = 25;
    shortBreak = 5;
    longBreak = 15;

    ui->ERR_MSG_S_P->setText("Reset succesful");
    ui->ERR_MSG_S_P->setStyleSheet("color: green;");
}

void MainWindow::on_Focus_duration_textChanged()
{
    if (ui->Focus_duration->text() == "25") {
        ui->ERR_MSG_S_P->setText("");
        return;
    }
    ui->ERR_MSG_S_P->setText("");
    if(ui->Focus_duration->text() == "") {
        ui->ERR_MSG_S_P->setText("Focus duration field cannot be empty.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validFocusTime = false;
        return;
    }
    QTime time = QTime::fromString(ui->Focus_duration->text(),"m");
    if (!time.isValid()) {
        ui->ERR_MSG_S_P->setText("Invalid Time Format. Focus duration should only consists of Minutes not greater than 60.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validFocusTime = false;
        return;
    }
    else if (time.minute() % 5 != 0) {
        ui->ERR_MSG_S_P->setText("Focus duration should be a multiple of 5");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validFocusTime = false;
        return;
    }
    validFocusTime = true;
}


void MainWindow::on_Short_duration_textChanged()
{
    if (ui->Short_duration->text() == "5") {
        ui->ERR_MSG_S_P->setText("");
        return;
    }
    ui->ERR_MSG_S_P->setText("");
    if(ui->Short_duration->text() == "") {
        ui->ERR_MSG_S_P->setText("Short Break duration field cannot be empty.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validShortBreak = false;
        return;
    }
    QTime ftime = QTime::fromString(ui->Focus_duration->text(),"m");
    QTime stime = QTime::fromString(ui->Short_duration->text(),"m");
    if (!stime.isValid()) {
        ui->ERR_MSG_S_P->setText("Invalid Time Format.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validShortBreak = false;
        return;
    }
    else if (stime.minute() != (ftime.minute() / 5)) {
        ui->ERR_MSG_S_P->setText("Short Break should be a 5th of the Focus Time.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validLongBreak = false;
        return;
    }
    validShortBreak = true;
}


void MainWindow::on_Long_duration_textChanged()
{
    if (ui->Long_duration->text() == "15") {
        ui->ERR_MSG_S_P->setText("");
        return;
    }
    ui->ERR_MSG_S_P->setText("");
    if(ui->Long_duration->text() == "") {
        ui->ERR_MSG_S_P->setText("Long Break duration field cannot be empty.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validLongBreak = false;
        return;
    }
    QTime stime = QTime::fromString(ui->Short_duration->text(),"m");
    QTime ltime = QTime::fromString(ui->Long_duration->text(),"m");
    if (!ltime.isValid()) {
        ui->ERR_MSG_S_P->setText("Invalid Time Format.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validLongBreak = false;
        return;
    }
    else if (ltime.minute() != (3 * stime.minute()) ) {
        ui->ERR_MSG_S_P->setText("Long Break should be 3/5th of the Focus Duration.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validLongBreak = false;
        return;
    }
    validLongBreak = true;
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
    // Variables for Name, Date, and Notes
    QString NAME = ui->Name_TB->text();
    QString NOTES = ui->Note_TB->toPlainText();
    QDate DATE = QDate::fromString(ui->Due_Date->text(), "dd/MM/yyyy");
    QDate c_date = QDate::currentDate();
    QString c_date_str = QDate::currentDate().toString("dd/MM/yyyy");
    QString d_date_str = DATE.toString("dd/MM/yyyy");

    // Setting status
    QString status;
    int count;
    if (c_date == DATE) {
        status = "In progress";
        count = 0;
    }
    else {
        count = abs(c_date.daysTo(DATE));
        qDebug() << count;
        if (count <= 3)
            status = "3 Day Mark";
        else if (count > 3 && count <= 5)
            status = "5 Day Mark";
        else if (count > 5 && count <= 7)
            status = "7 Day Mark";
        else if (count > 7)
            status = "Greater Than 7 Day Mark";
    }

    //VALIDATION
    if(DATE.isValid() && DATE >= QDate::currentDate()){
        if (Task_Manager->taskExists(NAME, c_date_str, d_date_str)) {
                ui->ERR_MSG_A_P->setText(
                    "Another task with the same name and dates already exists.");
                ui->ERR_MSG_A_P->setStyleSheet("color: red;");
        }
        else {
            if (Task_Manager->addTask(NAME, c_date_str, d_date_str, NOTES, count, status)) {
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

// Modify Task
void MainWindow::handleButtonClicked(const int id, QPushButton* button, bool isMPPage){
    if (prev_button != nullptr) {
        prev_button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #F1F1F1; min-width: 308px; min-height:20px; margin-bottom:20px;");
    }
    prev_button = button;
    button->setStyleSheet("color: #000000; border-radius: 6px; background-color: #FFDC81 ; min-width: 308px; min-height:20px; margin-bottom:20px;");
    if (isMPPage) {
        ui->name_M_P->setText(Task_Manager->getName(id));
        ui->c_date_M_P->setText(Task_Manager->getCreationDate(id));
        ui->d_date_M_P->setText(Task_Manager->getDueDate(id));
        ui->notes_M_P->setPlainText(Task_Manager->getNotes(id));
    }
    else  {
        ui->name_D_P->setText(Task_Manager->getName(id));
        ui->c_date_D_P->setText(Task_Manager->getCreationDate(id));
        ui->d_date_D_P->setText(Task_Manager->getDueDate(id));
        ui->notes_D_P->setText(Task_Manager->getNotes(id));
    }
    QString status = Task_Manager->getStatus(id);
    if (status == "In progress") {
        if (isMPPage) {
            ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/In Progress.png); image-position: left;");
            ui->stat_M_P_text->setText("");
        }
        else {
            ui->stat_D_P->setStyleSheet("image: url(:/Status/Status/In Progress.png); image-position: left;");
            ui->stat_D_P_text->setText("");
        }
    }
    else if (status == "Completed") {
        if (isMPPage) {
            ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/Completed_" + backgroundState + ".png); image-position: left;");
            ui->stat_M_P_text->setText("");
        }
        else {
            ui->stat_D_P->setStyleSheet("image: url(:/Status/Status/Completed_" + backgroundState + ".png); image-position: left;");
            ui->stat_D_P_text->setText("");
        }
    }
    else if (status == "3 Day Mark") {
        if (isMPPage) {
            ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/3 Day Mark.png); image-position: left;");
            ui->stat_M_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_M_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(254, 96, 125);");
        }
        else {
            ui->stat_D_P->setStyleSheet("image: url(:/Status/Status/3 Day Mark.png); image-position: left;");
            ui->stat_D_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_D_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(254, 96, 125);");
        }
    }
    else if (status == "5 Day Mark") {
        if (isMPPage) {
            ui->stat_M_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/5 Day Mark.png); image-position: left;");
            ui->stat_M_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(187, 101, 21);");
        }
        else {
            ui->stat_D_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_D_P->setStyleSheet("image: url(:/Status/Status/5 Day Mark.png); image-position: left;");
            ui->stat_D_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(187, 101, 21);");
        }
    }
    else if (status == "7 Day Mark") {
        if (isMPPage) {
            ui->stat_M_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/7 Day Mark.png); image-position: left;");
            ui->stat_M_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(184, 83, 219);");
        }
        else {
            ui->stat_D_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_D_P->setStyleSheet("image: url(:/Status/Status/7 Day Mark.png); image-position: left;");
            ui->stat_D_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(184, 83, 219);");
        }
    }
    else if (status == "Greater Than 7 Day Mark") {
        if (isMPPage) {
            ui->stat_M_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/Greater Than 7 Day Mark.png); image-position: left;");
            ui->stat_M_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(255, 255, 255);");
        }
        else {
            ui->stat_D_P_text->setText(intTOString(Task_Manager->getDifference(id)) + " Days left");
            ui->stat_D_P->setStyleSheet("image: url(:/Status/Status/Greater Than 7 Day Mark.png); image-position: left;");
            ui->stat_D_P_text->setStyleSheet("font: 600 10pt Manrope SemiBold; color: rgb(255, 255, 255);");
        }
    }
    current_selected_button = id;
}


void MainWindow::on_name_M_P_textChanged()
{
    ui->ERR_MSG_M_P->setText("");
    if(ui->name_M_P->text() == "" || ui->name_M_P->text().length() > 128){
        ui->ERR_MSG_M_P->setText("Name Field Empty or exceeds Character Limit(128).");
        ui->ERR_MSG_M_P->setStyleSheet("color: red;");
        validName = false;
        return;
    }
    validName = true;
}

void MainWindow::on_d_date_M_P_textChanged()
{
    QDate DATE = QDate::fromString(ui->d_date_M_P->text(), "dd/MM/yyyy");
    ui->ERR_MSG_M_P->setText("");
    if(!DATE.isValid() || DATE < QDate::currentDate() || ui->d_date_M_P->text() == ""){
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
            changed = true;
            layout_destruct(ui->scrollArea_M_P);
            Task_Manager->sortTasks();
            layout_creation(ui->scrollArea_M_P, true);
        }
    }
}

void MainWindow::on_stat_change_M_P_clicked()
{
    QString status = Task_Manager->getStatus(current_selected_button);
    if (status != "Completed") {
        ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/Completed_" + backgroundState + ".png); image-position: left;");
        ui->ERR_MSG_M_P->setText("Status of the Task has been set to Completed");
        ui->ERR_MSG_M_P->setStyleSheet("color: green;");
        Task_Manager->setStatus(current_selected_button, "Completed");
        ui->stat_M_P_text->setText("");
    }
    else {
        ui->ERR_MSG_M_P->setText("Status of the Task is already set to Completed");
        ui->ERR_MSG_M_P->setStyleSheet("color: red;");
    }
}

// Delete Task
void MainWindow::on_delete_btn_D_P_clicked()
{
    ui->ERR_MSG_D_P->setText("");
    if (deleteTaskCounter == 1)
        deleteTaskCounter++;
    if (deleteTaskCounter == 0) {
        QString name, c_date, date, notes;
        name = ui->name_D_P->text();
        c_date = ui->c_date_D_P->text();
        date = ui->d_date_D_P->text();
        notes = ui->notes_D_P->toPlainText();
        if(!Task_Manager->taskExists(name,c_date, date, notes)){
            ui->ERR_MSG_D_P->setText("Task has already been Deleted.");
            ui->ERR_MSG_D_P->setStyleSheet("color: red;");
        }
        else {
            deleteTaskCounter++;
            ui->ERR_MSG_D_P->setText("Press the Delete Button again to confirm deletion of selected Task.");
            ui->ERR_MSG_D_P->setStyleSheet("color: red;");
            return;
        }
    }
    else if (deleteTaskCounter == 2) {
        Task_Manager->deleteTask(current_selected_button);
        deleteTaskCounter = 0;
        ui->name_D_P->setText("");
        ui->c_date_D_P->setText("");
        ui->d_date_D_P->setText("");
        ui->notes_D_P->setText("");
        ui->ERR_MSG_D_P->setText("Operation Successful.");
        ui->ERR_MSG_D_P->setStyleSheet("color: green;");
        ui->stat_D_P->setStyleSheet("");
        ui->stat_D_P_text->setText("");
        layout_destruct(ui->scrollArea_D_P);
        layout_creation(ui->scrollArea_D_P, false);
    }
}

// Pomodoro

void MainWindow::updatePomodorotime()
{
    remainingTime--;
    time->setHMS(0, remainingTime / 60 , remainingTime % 60 );
    ui->Timer_Label->setText(time->toString("mm : ss"));
    if (remainingTime - 1 <= 0) {
        timer->stop();
        // returning back to normal focus period if short break is over or long break is over
        if (backToPomodoro())
            return;
        else {
            pomodoroCycles++;
            incrementPomodoroCycle();
        }
    }
}

bool MainWindow::backToPomodoro() {
    if (pomodoroCycles == 1 && shortBreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        shortBreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 2 && shortBreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle2->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        shortBreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 3 && shortBreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle2->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle3->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        shortBreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 4 && shortBreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle2->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle3->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
         ui->Pomodoro_cycle4->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        shortBreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 5 && shortBreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        shortBreakGiven = false;
        pomodoroCycles = 0;
        return true;
    }
    return false;
}
void MainWindow::incrementPomodoroCycle() {
    if (pomodoroCycles == 1 || pomodoroCycles == 2 || pomodoroCycles == 3 || pomodoroCycles == 4) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Short Break_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Short Break Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("");
        ui->Pomodoro_cycle2->setStyleSheet("");
        ui->Pomodoro_cycle3->setStyleSheet("");
        ui->Pomodoro_cycle4->setStyleSheet("");
        remainingTime = 60 * shortBreak;
        time->setHMS(0, shortBreak, 0, 0);
        ui->Timer_Label->setText(intTOString(shortBreak) + " : 00");
        shortBreakGiven = true;
    }
    else if (pomodoroCycles == 5) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Long Break_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Long Break Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("");
        ui->Pomodoro_cycle2->setStyleSheet("");
        ui->Pomodoro_cycle3->setStyleSheet("");
        ui->Pomodoro_cycle4->setStyleSheet("");
        remainingTime = 60 * longBreak;
        time->setHMS(0, longBreak, 0, 0);
        ui->Timer_Label->setText(intTOString(longBreak) + " : 00");
        shortBreakGiven = true;
    }
}

void MainWindow::on_Timer_btn_clicked()
{
    if (!timer->isActive()) {
        timer->start(1000);
        if (shortBreakGiven) {
            if (pomodoroCycles == 1 || pomodoroCycles == 2 || pomodoroCycles == 3 || pomodoroCycles == 4)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Short Break Pause.png);");
            else if (pomodoroCycles == 5)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Long Break Pause.png);");
        }
        else {
            if (pomodoroCycles == 0 || pomodoroCycles == 1 || pomodoroCycles == 2 || pomodoroCycles == 3 || pomodoroCycles == 4 || pomodoroCycles == 5)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Pause.png);");
        }
    }
    else {
        timer->stop();
        if (shortBreakGiven) {
            if (pomodoroCycles == 1 || pomodoroCycles == 2 || pomodoroCycles == 3 || pomodoroCycles == 4)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Short Break Play.png);");
            else if (pomodoroCycles == 5)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Long Break Play.png);");
        }
        else {
            if (pomodoroCycles == 0 || pomodoroCycles == 1 || pomodoroCycles == 2 || pomodoroCycles == 3 || pomodoroCycles == 4 || pomodoroCycles == 5)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        }
    }
}

void MainWindow::handlebuttonClickedWithId(int id, QScrollArea* scrollArea){
    qDebug() << "handlebutton invoked";
    QPushButton* buttonToClick = findButtonById(id, scrollArea);
    if (buttonToClick) {
        buttonToClick->click();
    }
}


QPushButton* MainWindow::findButtonById(int id, QScrollArea* scrollArea){
    qDebug() << "findBUtton Invoked";
    QString buttonName = QString("taskbutton_%1").arg(id);
    QWidget* scrollAreaWidget = scrollArea->widget();
    if(scrollAreaWidget){
        qDebug() << "BUtton FOund";
        return scrollArea->findChild<QPushButton*>(buttonName);
    }
    qDebug() << "BUtton not FOund";
    return nullptr;
}

void MainWindow::layout_TD_P_Today_create(QScrollArea *scrollArea, bool isToday){

    int count = 0;
    prev_button = nullptr;
    Task_Manager->CountTotalTasks(count);
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    scrollArea->setWidget(widget);
    int containerW = 1091, containerH = 46;
    widget->setMaximumSize(containerW, count * (containerH + 20));

    if(isToday){
        for (int i = 0; i < count; i++) {
            if(Task_Manager->getDifference(i) == 0 && Task_Manager->getStatus(i) != "Completed"){
                QWidget *container = new QWidget();
                QHBoxLayout *HLayout = new QHBoxLayout(container);
                QPushButton *button1 = new QPushButton();
                QPushButton *button2 = new QPushButton();
                QPushButton *button3 = new QPushButton();

                QLabel *label = new QLabel();


                label->setText(Task_Manager->getName(i));
                HLayout->addWidget(label);
                HLayout->addStretch(1);
                QLabel *status = new QLabel();
                status->setFixedWidth(190);
                status->setMaximumHeight(40);
                QString stat = Task_Manager->getStatus(i);
                QString days_left = intTOString(Task_Manager->getDifference(i)) + " Days Left";
                if (stat == "In progress") {
                    status->setText(stat);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(0,240,255,181.05); color:rgb(15,76,168); border:transparent; border-radius:10px;");

                }
                else if (stat == "Completed") {
                    status->setText(stat);
                    if(backgroundState == "Light"){
                        status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(50,213,131,102); color:rgb(70,199,145); border:transparent; border-radius:10px;");
                    }
                    else{
                        status->setStyleSheet("color:rgb(2,122,72); padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(50,213,131,26); border:transparent; border-radius:10px;");
                    }
                }
                else if (stat == "3 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(248,24,24,102); color:rgb(161,13,40); border:transparent; border-radius:10px;");
                }
                else if (stat == "5 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:#F2BE43; color:#BB6515; color:rgb(161,13,40); border:transparent; border-radius:10px;");

                }
                else if (stat == "7 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:#D4BAE0; color:#B853DB; color:rgb(161,13,40); border:transparent; border-radius:10px;");
                }
                else if (stat == "Greater Than 7 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:#1D1C1F; color:white; border:transparent; border-radius:10px;");
                }
                QIcon delete_icon(":/icons/icons/del_icon_" + backgroundState + ".png");
                QIcon modify_icon(":/icons/icons/modify_icon_" + backgroundState + ".png");
                QIcon status_icon(":/icons/icons/status_icon_" + backgroundState + ".png");
                button1->setIcon(delete_icon);
                button2->setIcon(modify_icon);
                button3->setIcon(status_icon);
                button1->setIconSize(QSize(28,33));
                button2->setIconSize(QSize(33,33));
                button3->setIconSize(QSize(34.03,33));

                button1->setStyleSheet("background-color:transparent; color:black; border: 0;");
                button2->setStyleSheet("background-color:transparent; color:black; border: 0;");
                button3->setStyleSheet("background-color:transparent; color:black; border: 0;");

                HLayout->addWidget(button1);
                HLayout->addWidget(button2);
                HLayout->addWidget(button3);
                HLayout->addWidget(status);
                container->setLayout(HLayout);
                label->setStyleSheet("font: 600 15pt Poppins; border: 0; color:black;");
                layout->addWidget(container);
                container->setStyleSheet("margin-bottom: 15px; margin-left: 5px; background-color:transparent; border: 1px solid rgba(87,87,87,127.5); border-radius:15px; margin-right:15px;");

                connect(button1, &QPushButton::clicked, this, [this, i]() {on_Delete_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_D_P);});
                connect(button2, &QPushButton::clicked, this, [this, i]() {on_Modify_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_M_P);});
                QString bgcolor;
                if(backgroundState == "Light"){bgcolor = "#b3eecc";} else bgcolor="#10773b";
                connect(button3, &QPushButton::clicked, this, [this, i, scrollArea, button3, bgcolor]() {button3->setStyleSheet("background-color:" + bgcolor + ";"); Task_Manager->setStatus(i, "Completed"); layout_TD_P_destruct(scrollArea); layout_TD_P_Today_create(scrollArea, true); });
            }
        }
        layout->insertStretch(-1, 1);
    }
    else{
        for (int i = 0; i < count; i++) {
            if(Task_Manager->getDifference(i) != 0 && Task_Manager->getStatus(i) != "Completed"){
                QWidget *container = new QWidget();
                QHBoxLayout *HLayout = new QHBoxLayout(container);
                QPushButton *button1 = new QPushButton();
                QPushButton *button2 = new QPushButton();
                QPushButton *button3 = new QPushButton();

                QLabel *label = new QLabel();
                label->setText(Task_Manager->getName(i));
                HLayout->addWidget(label);
                HLayout->addStretch(1);
                QLabel *status = new QLabel();
                status->setFixedWidth(200);
                status->setMaximumHeight(40);
                status->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                QString stat = Task_Manager->getStatus(i);
                QString days_left = intTOString(Task_Manager->getDifference(i)) + " Days Left";
                if (stat == "In progress") {
                    status->setText(stat);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(0,240,255,181.05); color:rgb(15,76,168); border:transparent; border-radius:10px;");

                }
                else if (stat == "Completed") {
                    status->setText(stat);
                    if(backgroundState == "Light"){
                        status->setStyleSheet("text-align:center; padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(50,213,131,102); color:rgb(70,199,145); border:transparent; border-radius:10px;");
                    }
                    else{
                        status->setStyleSheet("text-align:center; color:rgb(2,122,72); padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(50,213,131,26); border:transparent; border-radius:10px;");
                    }
                }
                else if (stat == "3 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:rgba(248,24,24,102); color:rgb(161,13,40); border:transparent; border-radius:10px;");
                }
                else if (stat == "5 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:#F2BE43; color:#BB6515; color:rgb(161,13,40); border:transparent; border-radius:10px;");

                }
                else if (stat == "7 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:#D4BAE0; color:#B853DB; color:rgb(161,13,40); border:transparent; border-radius:10px;");
                }
                else if (stat == "Greater Than 7 Day Mark") {
                    status->setText(days_left);
                    status->setStyleSheet("text-align:center; padding-left:50px; padding-right:50px; padding-top:0px; padding-bottom:0px; background-color:#1D1C1F; color:white; border:transparent; border-radius:10px;");
                }
                QIcon delete_icon(":/icons/icons/del_icon_" + backgroundState + ".png");
                QIcon modify_icon(":/icons/icons/modify_icon_" + backgroundState + ".png");
                QIcon status_icon(":/icons/icons/status_icon_" + backgroundState + ".png");
                button1->setIcon(delete_icon);
                button2->setIcon(modify_icon);
                button3->setIcon(status_icon);
                button1->setIconSize(QSize(28,33));
                button2->setIconSize(QSize(33,33));
                button3->setIconSize(QSize(34.03,33));
                button1->setStyleSheet(" background-color:transparent; color:black; border: 0;");
                button2->setStyleSheet(" background-color:transparent; color:black; border: 0;");
                button3->setStyleSheet(" background-color:transparent; color:black; border: 0;");

                HLayout->addWidget(button1);
                HLayout->addWidget(button2);
                HLayout->addWidget(button3);
                HLayout->addWidget(status);
                container->setLayout(HLayout);
                container->setStyleSheet("margin-bottom: 15px; margin-left: 5px; background-color:transparent; border: 1px solid rgba(87,87,87,127.5); border-radius:15px; margin-right:15px;");
                label->setStyleSheet("font: 600 15pt Poppins; border: 0; color:black;");
                layout->addWidget(container);

                connect(button1, &QPushButton::clicked, this, [this, i]() {on_Delete_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_D_P);});
                connect(button2, &QPushButton::clicked, this, [this, i]() {on_Modify_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_M_P);});
                QString bgcolor;
                if(backgroundState == "Light"){bgcolor = "#b3eecc";} else bgcolor="#10773b";
                connect(button3, &QPushButton::clicked, this, [this, i,scrollArea, button3, bgcolor]() {button3->setStyleSheet("background-color:" + bgcolor + ";"); Task_Manager->setStatus(i, "Completed"); layout_TD_P_destruct(scrollArea); layout_TD_P_Today_create(scrollArea, false); });
            }
        }
        layout->insertStretch(-1, 1);
    }
}

void MainWindow::layout_TD_P_destruct(QScrollArea *scrollArea){
    QWidget* widget = scrollArea->widget();
        if (widget) {
            QLayout* layout = widget->layout();
                if (layout) {
                    // Iterate through the layout items (containers)
                while (QLayoutItem* item = layout->takeAt(0)) {
                    if (QWidget* container = item->widget()) {
                        QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(container->layout());
                        if (hLayout) {
                            // Iterate through the container's items (label, buttons, status)
                            while (QLayoutItem* childItem = hLayout->takeAt(0)) {
                                if (QWidget* childWidget = childItem->widget()) {
                                    delete childWidget; // Delete buttons, label, status
                                }
                                delete childItem;
                            }
                            delete hLayout; // Delete the QHBoxLayout
                        }
                        delete container; // Delete the container widget
                    }
                    delete item; // Delete the layout item
                }
                delete layout; // Delete the QVBoxLayout
            }
            delete widget; // Delete the main widget
            scrollArea->setWidget(nullptr);
        }
}

