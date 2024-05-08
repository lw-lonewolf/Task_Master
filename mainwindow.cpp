#include "mainwindow.h"
#include "ui_mainwindow.h"
//-- Included for Locating Children --//
#include <QDebug>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
#include <QVBoxLayout>
//-- Date Functionality --//
#include <QDate>
//-- Dialog Functionality --//
#include <QMessageBox>

MainWindow::MainWindow(DbManager& obj, QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // intiliasting private data member to point to the db object
    Task_Manager = &obj;
    Task_Manager->createTasksTable();
    Task_Manager->createUsersTable();

    // starting with the login screen as program launches
    ui->TaskBar->setDisabled(true);
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Sign In Page.png);");
    ui->stackedWidget->setCurrentWidget(ui->Login_Page);
    ui->Search_btn_taskbar->setDisabled(true);
    ui->Search_btn_taskbar->setVisible(false);
    validLoginUsername = false;
    validLoginPassword = false;
    validRegisterUsername = false;
    validRegisterPassword = false;
    validRegisterConfirmPassword = false;

    // initialising values for backgrounds
    backgroundChanged = false;


    // initialising variables for pomodoro page
    validFocusTime = false;
    validShortBreak = false;
    validLongBreak = false;
    BreakGiven = false;
    pomodoroCycles = 0;
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePomodorotime);

    // initialising variables for modify page
    validName = false;
    validDDate = false;
    validNotes = false;
    changed = false;

    // initialising varibles for delete page
    deleteTaskCounter = 0;

    // initiasling variables for search page
    validSearchName = false;
    validSearchDate = false;

    this->setStyleSheet("QPushButton:focus { outline: none; border:none; }");
}

MainWindow::~MainWindow() { delete ui; }

// Helper functions
QString MainWindow::intTOString(int num) {
    QString result;
    if (num == 0)
        return "0";
    while (num != 0) {
        int digit = num % 10;
        char asciiChar = '0' + digit;
        result = asciiChar + result;
        num /= 10;
    }

    return result;
}

// Used to create layout for both Delete Task page and Modify Task page
void MainWindow::layout_creation(QScrollArea* scrollArea, bool isMPPage){
    int count = 0;
    prev_button = nullptr;
    Task_Manager->CountTotalTasks(count, user_id);
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    scrollArea->setWidget(widget);
    int buttonW = 308, buttonH = 20;

    widget->setMinimumSize(buttonW, count*(buttonH+20));
    for(int i = 0; i < count; i++){
        QPushButton *button = new QPushButton(Task_Manager->getName(i, user_id));
        button->setObjectName(QString("taskbutton_%1").arg(i));
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, i, button, isMPPage]() {handleButtonClicked(i,button, isMPPage);} );
        button->setStyleSheet("font: 600 14pt Poppins; color: #000000; border-radius: 6px; background-color: #F1F1F1; min-width: 308px; min-height:20px; margin-bottom: 20px;");
        if (i == current_selected_button && changed) {
            button->setStyleSheet("font: 600 14pt Poppins; color: #000000; border-radius: 6px; background-color: #FFDC81 ; min-width: 308px; min-height:20px; margin-bottom:20px;");
            prev_button = button;
        }
    }
    layout->insertStretch(-1,1);
}

// Used to delete layout for both Delete Task page and Modify Task page
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

// Used to determine and set appropriate status where needed for a given widget and its text.
void MainWindow::implementStatus (const int id, const int user_id, QWidget*& statusWidget, QLabel*& statusText) {
    QString status = Task_Manager->getStatus(id, user_id);
    if (status == "In progress") {
        statusText->setText("");
        statusText->setStyleSheet("image: none;");
        statusWidget->setStyleSheet("image: url(:/Status/Status/In Progress.png); image-position: left; border: 0");
    }
    else if (status == "Completed") {
        statusText->setText("");
        statusText->setStyleSheet("image: none;");
        statusWidget->setStyleSheet("image: url(:/Status/Status/Completed_" + backgroundState + ".png); image-position: left; border: 0");
    }
    else if (status == "3 Day Mark") {
        statusText->setText(intTOString(Task_Manager->getDifference(id, user_id)) + " Days left");
        statusWidget->setStyleSheet("image: url(:/Status/Status/3 Day Mark.png); image-position: left; border: 0");
        statusText->setStyleSheet("font: 800 10pt Manrope ExtraBold; color: rgb(255, 0, 0); image: none;");
    }
    else if (status == "5 Day Mark") {
        statusText->setText(intTOString(Task_Manager->getDifference(id, user_id)) + " Days left");
        statusWidget->setStyleSheet("image: url(:/Status/Status/5 Day Mark.png); image-position: left; border: 0");
        statusText->setStyleSheet("font: 800 10pt Manrope ExtraBold; color: rgb(187, 101, 21); image: none;");
    }
    else if (status == "7 Day Mark") {
        statusText->setText(intTOString(Task_Manager->getDifference(id, user_id)) + " Days left");
        statusWidget->setStyleSheet("image: url(:/Status/Status/7 Day Mark.png); image-position: left; border: 0");
        statusText->setStyleSheet("font: 800 10pt Manrope ExtraBold; color: rgb(184, 83, 219); image: none;");
    }
    else if (status == "Greater Than 7 Day Mark") {
        statusText->setText(intTOString(Task_Manager->getDifference(id, user_id)) + " Days left");
        statusWidget->setStyleSheet("image: url(:/Status/Status/Greater Than 7 Day Mark.png); image-position: left; border: 0");
        statusText->setStyleSheet("font: 800 10pt Manrope ExtraBold; color: rgb(255, 255, 255); image: none;");
    }
}

void MainWindow::changeFontColor() {
    if (backgroundState == "Light") {
        fontcolor = "color: rgb(0, 0, 0);";
        this->setStyleSheet("QLabel {" + fontcolor + "}" +"QPushButton {" + fontcolor + "}" + "QLineEdit {" + fontcolor + "}" + "QPushButton:focus { outline: none; }" + "QPlainTextEdit {" + fontcolor + "}" + "QTextBrowser {" + fontcolor + "}");
    }
    else {
        fontcolor = "color: rgb(244, 240, 240);";
        this->setStyleSheet("QLabel {" + fontcolor + "}" +"QPushButton {" + fontcolor + "}" + "QLineEdit {" + fontcolor + "}" + "QPushButton:focus { outline: none; }" + "QPlainTextEdit {" + fontcolor + "}" + "QTextBrowser {" + fontcolor + "}");
    }
}

// SideBar
void MainWindow::on_Home_btn_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Home_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Home_" + backgroundState +
            ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Home_Page);
    }
    ui->Home_username->setText(Task_Manager->getUserName(user_id));
    QString hoursWorked;
    hoursWorked.setNum(Task_Manager->getHoursWorked(user_id), 'f', 2);
    ui->Home_hoursWorked->setText(hoursWorked);
    ui->Home_TasksDone->setText(intTOString(Task_Manager->getTasksDone(user_id)));
    int count = 0;
    Task_Manager->CountTotalTasks(count, user_id);

    ui->Home_Task1_Status->setStyleSheet("");
    ui->Home_Task1_Status_text->setText("");

    ui->Home_Task2_Status->setStyleSheet("");
    ui->Home_Task2_Status_text->setText("");

    ui->Home_Task3_Status->setStyleSheet("");
    ui->Home_Task3_Status_text->setText("");

    ui->Home_Task4_Status->setStyleSheet("");
    ui->Home_Task4_Status_text->setText("");
    if (count == 0) {
        ui->Home_Task1->setText("No Task");
        ui->Home_Task2->setText("No Task");
        ui->Home_Task3->setText("No Task");
        ui->Home_Task4->setText("No Task");
    }
    else if (count == 1) {
        ui->Home_Task1->setText(Task_Manager->getName(0,user_id));
        ui->Home_Task2->setText("No Task");
        ui->Home_Task3->setText("No Task");
        ui->Home_Task4->setText("No Task");

        implementStatus(0, user_id, ui->Home_Task1_Status, ui->Home_Task1_Status_text);
    }
    else if (count == 2) {
        ui->Home_Task1->setText(Task_Manager->getName(0,user_id));
        ui->Home_Task2->setText(Task_Manager->getName(1,user_id));
        ui->Home_Task3->setText("No Task");
        ui->Home_Task4->setText("No Task");

        implementStatus(0, user_id, ui->Home_Task1_Status, ui->Home_Task1_Status_text);
        implementStatus(1, user_id, ui->Home_Task2_Status, ui->Home_Task2_Status_text);
    }
    else if (count == 3) {
        ui->Home_Task1->setText(Task_Manager->getName(0,user_id));
        ui->Home_Task2->setText(Task_Manager->getName(1,user_id));
        ui->Home_Task3->setText(Task_Manager->getName(2,user_id));
        ui->Home_Task4->setText("No Task");

        implementStatus(0, user_id, ui->Home_Task1_Status, ui->Home_Task1_Status_text);
        implementStatus(1, user_id, ui->Home_Task2_Status, ui->Home_Task2_Status_text);
        implementStatus(2, user_id, ui->Home_Task3_Status, ui->Home_Task3_Status_text);
    }
    else if (count >= 4) {
        ui->Home_Task1->setText(Task_Manager->getName(0,user_id));
        ui->Home_Task2->setText(Task_Manager->getName(1,user_id));
        ui->Home_Task3->setText(Task_Manager->getName(2,user_id));
        ui->Home_Task4->setText(Task_Manager->getName(3,user_id));

        implementStatus(0, user_id, ui->Home_Task1_Status, ui->Home_Task1_Status_text);
        implementStatus(1, user_id, ui->Home_Task2_Status, ui->Home_Task2_Status_text);
        implementStatus(2, user_id, ui->Home_Task3_Status, ui->Home_Task3_Status_text);
        implementStatus(3, user_id, ui->Home_Task4_Status, ui->Home_Task4_Status_text);
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
    if(backgroundState == "Light"){
        ui->calendarWidget->setStyleSheet("QCalendarWidget QWidget {background-color:#CBCACA; alternate-background-color: #BBBBBB;} QCalendarWidget QAbstractItemView:disabled{font-size: 12pt; color:#918E8E;} QCalendarWidget QAbstractItemView:enabled {font-size:12pt; color: #252525; selection-background-color: #CBCACA;selection-color: #252525;}");
    }
    else{
        ui->calendarWidget->setStyleSheet("QCalendarWidget QWidget {background-color:#434141; alternate-background-color: #4E4D4D;} QCalendarWidget QAbstractItemView:disabled{font-size: 12pt; color:#817D7D;} QCalendarWidget QAbstractItemView:enabled {font-size:12pt; color: #F4F0F0; selection-background-color: #434141;selection-color: #F4F0F0;}");
    }
    QDate currentdate = QDate::currentDate();
    // MainWindow::on_calendarWidget_currentPageChanged(currentdate.year(), currentdate.month());
    ui->calendarWidget->setCurrentPage(currentdate.year(), currentdate.month());
    QString date = currentdate.toString("MMM, yyyy");
    ui->Calendar_text->setText(date);
}

void MainWindow::on_Pomodoro_btn_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Pomodoro_Page && !BreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Pomodoro_Page);
    }
    else if (BreakGiven) {
        ui->stackedWidget->setCurrentWidget(ui->Pomodoro_Page);
        incrementPomodoroCycle();
        if (timer->isActive()) {
            if (pomodoroCycles == 1 || pomodoroCycles == 2 || pomodoroCycles == 3 || pomodoroCycles == 4)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Short Break Pause.png);");
            else if (pomodoroCycles == 5)
                ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Long Break Pause.png);");
        }
    }
    if (ui->Timer_Label->text() == "") {
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
    }
}

void MainWindow::on_Settings_btn_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Settings_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Settings_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
        ui->ERR_MSG_S_P->setText("");
    }
    ui->Focus_duration->setText(intTOString(focusTime));
    ui->Short_duration->setText(intTOString(shortBreak));
    ui->Long_duration->setText(intTOString(longBreak));

}

void MainWindow::on_Search_btn_taskbar_clicked()
{
    if (ui->stackedWidget->currentWidget() != ui->Search_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Search_Layout_" + backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Search_Page);
        MainWindow::on_Search_back_btn_clicked();
        layout_Search_destroy();
        ui->Search_Name_User->setText("");
        ui->Search_Creation_Date_User->setText("");
        ui->ERR_MSG_Search->setText("");
    }
    if (ui->BackGround->styleSheet() == "background-image: url(:/Background_Images/Search_" + backgroundState + ".png);") {
        MainWindow::on_Search_back_btn_clicked();
    }
}

void MainWindow::on_LogOut_btn_clicked()
{
    ui->TaskBar->setDisabled(true);
    ui->Search_btn_taskbar->setDisabled(true);
    ui->Search_btn_taskbar->setVisible(false);
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Sign In Page.png);");
    ui->stackedWidget->setCurrentWidget(ui->Login_Page);
    user_id = 0;
    ui->Login_username->setText("");
    ui->Login_password->setText("");
    ui->ERR_MSG_L_P->setText("");
    fontcolor = "color: rgb(0, 0, 0);";
    ui->Login_username->setStyleSheet(fontcolor);
    ui->Login_password->setStyleSheet(fontcolor);
    ui->Register_username->setStyleSheet(fontcolor);
    ui->Register_password->setStyleSheet(fontcolor);
    ui->Register_confirmPassword->setStyleSheet(fontcolor);

    if(timer->isActive())
        timer->stop();
    ui->Timer_Label->setText("");
    ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
    time->setHMS(0, focusTime, 0, 0);
    remainingTime = 60 * focusTime;
    BreakGiven = false;
    pomodoroCycles = 0;
}

// Home
void MainWindow::on_View_More_clicked() {
    MainWindow::on_ToDo_List_btn_clicked();
}

void MainWindow::on_P_Button_clicked() {
    MainWindow::on_Pomodoro_btn_clicked();
}

void MainWindow::on_Add_Task_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Add_Task_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/New Task_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Add_Task_Page);
        ui->Name_TB->setText("");
        ui->Due_Date->setText("");
        ui->Note_TB->setPlainText("");
        ui->ERR_MSG_A_P->setText("");
    }
}

void MainWindow::on_Delete_Task_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Delete_Task_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Delete Task_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Delete_Task_Page);
        ui->notes_D_P->setText("");
        ui->name_D_P->setText("");
        ui->d_date_D_P->setText("");
        ui->c_date_D_P->setText("");
        ui->ERR_MSG_D_P->setText("");
        ui->stat_D_P_text->setText("");
        ui->stat_D_P->setStyleSheet("");
    }
    layout_creation(ui->scrollArea_D_P, false);
}

void MainWindow::on_Modify_Task_clicked() {
    if (ui->stackedWidget->currentWidget() != ui->Modify_Task_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Modify Task_" +
            backgroundState + ".png);");
        ui->stackedWidget->setCurrentWidget(ui->Modify_Task_Page);
        ui->notes_M_P->setPlainText("");
        ui->name_M_P->setText("");
        ui->d_date_M_P->setText("");
        ui->c_date_M_P->setText("");
        ui->ERR_MSG_M_P->setText("");
        ui->stat_M_P_text->setText("");
        ui->stat_M_P->setStyleSheet("");
    }
    layout_creation(ui->scrollArea_M_P, true);

}

// Settings
void MainWindow::on_Light_Button_clicked() {
    if (ui->BackGround->styleSheet() != "background-image: url(:/Background_Images/Settings_Light.png);") {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Settings_Light.png);");
        backgroundChanged = true;
    }
    else
        backgroundChanged = false;
}

void MainWindow::on_Dark_Button_clicked() {
    if (ui->BackGround->styleSheet() != "background-image: url(:/Background_Images/Settings_Dark.png);") {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Settings_Dark.png);");
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

        if (ui->BackGround->styleSheet() == "background-image: url(:/Background_Images/Settings_Light.png);") {
            backgroundState = "Light";
        } else
            backgroundState = "Dark";
        Task_Manager->setTheme(user_id,backgroundState);
        Task_Manager->setfocusTime(user_id, focusTime);
        Task_Manager->setshortTime(user_id,shortBreak);
        Task_Manager->setlongTime(user_id,longBreak);
        changeFontColor();
        backgroundChanged = false;
        if(timer->isActive())
            timer->stop();
        ui->Timer_Label->setText("");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        time->setHMS(0, focusTime, 0, 0);
        remainingTime = 60 * focusTime;

        validFocusTime = false;
        validShortBreak = false;
        validLongBreak = false;
    }
    else {
        if (backgroundChanged) {
            if (ui->BackGround->styleSheet() ==
                "background-image: url(:/Background_Images/Settings_Light.png);") {
                backgroundState = "Light";
            } else
                backgroundState = "Dark";
            Task_Manager->setTheme(user_id,backgroundState);
            changeFontColor();
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
            Task_Manager->setfocusTime(user_id, focusTime);
            Task_Manager->setshortTime(user_id,shortBreak);
            Task_Manager->setlongTime(user_id,longBreak);
            ui->ERR_MSG_S_P->setText("Saved sucessfully");
            ui->ERR_MSG_S_P->setStyleSheet("color: green;");
            if(timer->isActive())
                timer->stop();
            ui->Timer_Label->setText("");
            ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
            time->setHMS(0, focusTime, 0, 0);
            remainingTime = 60 * focusTime;

            validFocusTime = false;
            validShortBreak = false;
            validLongBreak = false;
        }
    }
}

void MainWindow::on_Reset_Button_clicked() {
    if (backgroundState == "Light" && focusTime == 25 && shortBreak == 5 && longBreak == 15) {
        ui->ERR_MSG_S_P->setText("Reset already");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        return;
    }
    else if (focusTime != 25 && shortBreak != 5 && longBreak != 15) {
        if(timer->isActive())
            timer->stop();
        ui->Timer_Label->setText("");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        time->setHMS(0, 25, 0, 0);
        remainingTime = 60 * 25;
    }
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

    Task_Manager->setTheme(user_id,backgroundState);
    Task_Manager->setfocusTime(user_id, focusTime);
    Task_Manager->setshortTime(user_id,shortBreak);
    Task_Manager->setlongTime(user_id,longBreak);
    changeFontColor();
    ui->ERR_MSG_S_P->setText("Reset succesful");
    ui->ERR_MSG_S_P->setStyleSheet("color: green;");

    validFocusTime = false;
    validShortBreak = false;
    validLongBreak = false;
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
    QTime time;
    int minute = 0;
    if (ui->Focus_duration->text() == "60") {
        time.setHMS(1,0,0);
        minute = 60;
    }
    else {
        time = QTime::fromString(ui->Focus_duration->text(),"m");
        minute = time.minute();
    }
    if (!time.isValid()) {
        ui->ERR_MSG_S_P->setText("Invalid Time Format. Focus duration should only consists of Minutes not greater than 60.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validFocusTime = false;
        return;
    }
    else if (minute % 5 != 0 || minute == 0) {
        ui->ERR_MSG_S_P->setText("Focus duration should be a multiple of 5 and cannot be zero");
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
    QTime ftime;
    int minute = 0;
    if (ui->Focus_duration->text() == "60") {
        ftime.setHMS(1,0,0);
        minute = 60;
    }
    else {
        ftime = QTime::fromString(ui->Focus_duration->text(),"m");
        minute = ftime.minute();
    }
    QTime stime = QTime::fromString(ui->Short_duration->text(),"m");
    if (!stime.isValid()) {
        ui->ERR_MSG_S_P->setText("Invalid Time Format.");
        ui->ERR_MSG_S_P->setStyleSheet("color: red;");
        validShortBreak = false;
        return;
    }
    else if (stime.minute() != (minute / 5) || stime.minute() == 0) {
        ui->ERR_MSG_S_P->setText("Short Break should be a 5th of the Focus Time and cannot be zero.");
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
    else if (ltime.minute() != (3 * stime.minute()) || ltime.minute() == 0) {
        ui->ERR_MSG_S_P->setText("Long Break should be 3/5th of the Focus Duration and cannot be zero.");
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
        if (Task_Manager->taskExists(NAME, c_date_str, d_date_str, user_id)) {
                ui->ERR_MSG_A_P->setText(
                    "Another task with the same name and dates already exists.");
                ui->ERR_MSG_A_P->setStyleSheet("color: red;");
        }
        else {
            if (Task_Manager->addTask(NAME, c_date_str, d_date_str, NOTES, status, count, user_id)) {
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
    Task_Manager->sortTasks();
}

// Modify Task
void MainWindow::handleButtonClicked(const int id, QPushButton* button, bool isMPPage){
    if (prev_button != nullptr) {
        prev_button->setStyleSheet("font: 600 14pt Poppins; color: #000000; border-radius: 6px; background-color: #F1F1F1; min-width: 308px; min-height:20px; margin-bottom:20px;");
    }
    prev_button = button;
    button->setStyleSheet("font: 600 14pt Poppins; color: #000000; border-radius: 6px; background-color: #FFDC81 ; min-width: 308px; min-height:20px; margin-bottom:20px;");
    if (isMPPage) {
        ui->name_M_P->setText(Task_Manager->getName(id, user_id));
        ui->c_date_M_P->setText(Task_Manager->getCreationDate(id, user_id));
        ui->d_date_M_P->setText(Task_Manager->getDueDate(id, user_id));
        ui->notes_M_P->setPlainText(Task_Manager->getNotes(id, user_id));
        implementStatus(id, user_id, ui->stat_M_P, ui->stat_M_P_text);
    }
    else  {
        ui->name_D_P->setText(Task_Manager->getName(id, user_id));
        ui->c_date_D_P->setText(Task_Manager->getCreationDate(id, user_id));
        ui->d_date_D_P->setText(Task_Manager->getDueDate(id, user_id));
        ui->notes_D_P->setText(Task_Manager->getNotes(id, user_id));
        implementStatus(id, user_id, ui->stat_D_P, ui->stat_D_P_text);
    }
    current_selected_button = id;
    deleteTaskCounter = 0;
    ui->ERR_MSG_D_P->setText("");
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
        if(Task_Manager->taskExists(name,c_date, date, user_id, notes)){
            ui->ERR_MSG_M_P->setText("Task Already exists with same Details.");
            ui->ERR_MSG_M_P->setStyleSheet("color: red;");
        }
        else{
            ui->ERR_MSG_M_P->setText("Operation Successful");
            ui->ERR_MSG_M_P->setStyleSheet("color:green;");
            Task_Manager->setName(current_selected_button, user_id , name);
            Task_Manager->setDueDate(current_selected_button, user_id, date);
            Task_Manager->setNotes(current_selected_button, user_id, notes);
            changed = true;
            layout_destruct(ui->scrollArea_M_P);
            Task_Manager->sortTasks();
            layout_creation(ui->scrollArea_M_P, true);

            validName = false;
            validDDate = false;
            validNotes = false;
            changed = false;
        }
    }
}

void MainWindow::on_stat_change_M_P_clicked()
{
    QString status = Task_Manager->getStatus(current_selected_button, user_id);
    if (status != "Completed") {
        ui->stat_M_P->setStyleSheet("image: url(:/Status/Status/Completed_" + backgroundState + ".png); image-position: left;");
        ui->ERR_MSG_M_P->setText("Status of the Task has been set to Completed");
        ui->ERR_MSG_M_P->setStyleSheet("color: green;");
        Task_Manager->setStatus(current_selected_button, user_id, "Completed");
        ui->stat_M_P_text->setText("");
        Task_Manager->setTasksDone(user_id, Task_Manager->getTasksDone(user_id) + 1);
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
        if(!Task_Manager->taskExists(name,c_date, date, user_id, notes)){
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
        Task_Manager->deleteTask(current_selected_button, user_id);
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
    Task_Manager->sortTasks();
}

// Pomodoro

void MainWindow::updatePomodorotime()
{
    remainingTime--;
    time->setHMS(0, remainingTime / 60 , remainingTime % 60 );
    ui->Timer_Label->setText(time->toString("mm : ss"));
    if (remainingTime - 1 <= 0) {
        if (!BreakGiven)
            Task_Manager->setHoursWorked(user_id, Task_Manager->getHoursWorked(user_id) + float(focusTime / 60.0));
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
    if (pomodoroCycles == 1 && BreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        BreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 2 && BreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle2->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        BreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 3 && BreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle2->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle3->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        BreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 4 && BreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        ui->Pomodoro_cycle1->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle2->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        ui->Pomodoro_cycle3->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
         ui->Pomodoro_cycle4->setStyleSheet("image: url(:/Background_Images/pomodoro checkbox.png); image-position: center;");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        BreakGiven = false;
        return true;
    }
    else if (pomodoroCycles == 5 && BreakGiven) {
        ui->BackGround->setStyleSheet("background-image: url(:/Background_Images/Pomodoro_" + backgroundState + ".png);");
        ui->Timer_btn->setStyleSheet("image: url(:/Background_Images/Pomodoro Play.png);");
        remainingTime = 60 * focusTime;
        time->setHMS(0, focusTime, 0, 0);
        ui->Timer_Label->setText(intTOString(focusTime) + " : 00");
        BreakGiven = false;
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
        BreakGiven = true;
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
        BreakGiven = true;
    }
}

void MainWindow::on_Timer_btn_clicked()
{
    if (!timer->isActive()) {
        timer->start(1000);
        if (BreakGiven) {
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
        if (BreakGiven) {
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

// To-Do List
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
        qDebug() << "Button Found";
        return scrollArea->findChild<QPushButton*>(buttonName);
    }
    qDebug() << "Button not Found";
    return nullptr;
}

void MainWindow::layout_TD_P_Today_create(QScrollArea *scrollArea, bool isToday){

    int count = 0;
    prev_button = nullptr;
    Task_Manager->CountTotalTasks(count, user_id);
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    scrollArea->setWidget(widget);
    int containerW = 1100, containerH = 60;
    widget->setMaximumSize(containerW, count * (containerH + 20));

    if(isToday){
        for (int i = 0; i < count; i++) {
            if(Task_Manager->getDifference(i, user_id) == 0 && Task_Manager->getStatus(i, user_id) != "Completed"){
                QWidget *container = new QWidget();
                QHBoxLayout *HLayout = new QHBoxLayout(container);
                QPushButton *button1 = new QPushButton();
                QPushButton *button2 = new QPushButton();
                QPushButton *button3 = new QPushButton();

                QLabel *label = new QLabel();
                label->setText(Task_Manager->getName(i, user_id));
                label->setStyleSheet("font: 600 14pt Poppins; border: 0;");
                HLayout->addWidget(label);
                HLayout->addStretch(1);

                QWidget *status = new QWidget();
                QHBoxLayout* status_layout = new QHBoxLayout(status);
                status->setFixedWidth(200);
                status->setMaximumHeight(61);
                QLabel *status_text = new QLabel();
                status_layout->addStretch();
                status_layout->addWidget(status_text);
                status_layout->addStretch();
                implementStatus(i, user_id, status, status_text);

                QIcon delete_icon(":/icons/icons/del_icon_" + backgroundState + ".png");
                QIcon modify_icon(":/icons/icons/modify_icon_" + backgroundState + ".png");
                QIcon status_icon(":/icons/icons/status_icon_" + backgroundState + ".png");
                button1->setIcon(delete_icon);
                button2->setIcon(modify_icon);
                button3->setIcon(status_icon);
                button1->setIconSize(QSize(24,30));
                button2->setIconSize(QSize(30,30));
                button3->setIconSize(QSize(32,31));

                button1->setStyleSheet("background-color:transparent; color:black; border: 0;");
                button2->setStyleSheet("background-color:transparent; color:black; border: 0;");
                button3->setStyleSheet("background-color:transparent; color:black; border: 0;");

                HLayout->addWidget(button1);
                HLayout->addWidget(button2);
                HLayout->addWidget(button3);
                HLayout->addWidget(status);
                container->setLayout(HLayout);
                layout->addWidget(container);
                container->setStyleSheet("margin-bottom: 15px; margin-left: 5px; background-color:transparent; border: 1px solid rgba(87,87,87,127.5); border-radius:15px; margin-right:15px;");

                connect(button1, &QPushButton::clicked, this, [this, i]() {on_Delete_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_D_P);});
                connect(button2, &QPushButton::clicked, this, [this, i]() {on_Modify_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_M_P);});
                QString bgcolor;
                if(backgroundState == "Light"){bgcolor = "#b3eecc";} else bgcolor="#10773b";
                connect(button3, &QPushButton::clicked, this, [this, i, scrollArea, button3, bgcolor]() {button3->setStyleSheet("background-color:" + bgcolor + ";"); Task_Manager->setStatus(i,user_id, "Completed"); layout_TD_P_destruct(scrollArea); layout_TD_P_Today_create(scrollArea, true); Task_Manager->setTasksDone(user_id, Task_Manager->getTasksDone(user_id) + 1); });
            }
        }
        layout->insertStretch(-1, 1);
    }
    else{
        for (int i = 0; i < count; i++) {
            if(Task_Manager->getDifference(i,user_id) != 0 && Task_Manager->getStatus(i, user_id) != "Completed"){
                QWidget *container = new QWidget();
                QHBoxLayout *HLayout = new QHBoxLayout(container);
                QPushButton *button1 = new QPushButton();
                QPushButton *button2 = new QPushButton();
                QPushButton *button3 = new QPushButton();

                QLabel *label = new QLabel();
                label->setText(Task_Manager->getName(i, user_id));
                label->setStyleSheet("font: 600 14pt Poppins; border: 0;");
                HLayout->addWidget(label);
                HLayout->addStretch(1);

                QWidget *status = new QWidget();
                QHBoxLayout* status_layout = new QHBoxLayout(status);
                status->setFixedWidth(200);
                status->setMaximumHeight(61);
                QLabel *status_text = new QLabel();
                status_layout->addStretch();
                status_layout->addWidget(status_text);
                status_layout->addStretch();
                implementStatus(i, user_id, status, status_text);

                QIcon delete_icon(":/icons/icons/del_icon_" + backgroundState + ".png");
                QIcon modify_icon(":/icons/icons/modify_icon_" + backgroundState + ".png");
                QIcon status_icon(":/icons/icons/status_icon_" + backgroundState + ".png");
                button1->setIcon(delete_icon);
                button2->setIcon(modify_icon);
                button3->setIcon(status_icon);
                button1->setIconSize(QSize(24,30));
                button2->setIconSize(QSize(30,30));
                button3->setIconSize(QSize(32,31));
                button1->setStyleSheet(" background-color:transparent; color:black; border: 0;");
                button2->setStyleSheet(" background-color:transparent; color:black; border: 0;");
                button3->setStyleSheet(" background-color:transparent; color:black; border: 0;");

                HLayout->addWidget(button1);
                HLayout->addWidget(button2);
                HLayout->addWidget(button3);
                HLayout->addWidget(status);
                container->setLayout(HLayout);
                container->setStyleSheet("margin-bottom: 15px; margin-left: 5px; background-color:transparent; border: 1px solid rgba(87,87,87,127.5); border-radius:15px; margin-right:15px;");
                layout->addWidget(container);

                connect(button1, &QPushButton::clicked, this, [this, i]() {on_Delete_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_D_P);});
                connect(button2, &QPushButton::clicked, this, [this, i]() {on_Modify_Task_clicked(); handlebuttonClickedWithId(i, ui->scrollArea_M_P);});
                QString bgcolor;
                if(backgroundState == "Light"){bgcolor = "#b3eecc";} else bgcolor="#10773b";
                connect(button3, &QPushButton::clicked, this, [this, i,scrollArea, button3, bgcolor]() {button3->setStyleSheet("background-color:" + bgcolor + ";"); Task_Manager->setStatus(i, user_id, "Completed"); layout_TD_P_destruct(scrollArea); layout_TD_P_Today_create(scrollArea, false); Task_Manager->setTasksDone(user_id, Task_Manager->getTasksDone(user_id) + 1); });
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

// Login Page
void MainWindow::on_Login_username_textChanged()
{
    ui->ERR_MSG_L_P->setText("");
    if (ui->Login_username->text() == "") {
        ui->ERR_MSG_L_P->setText("Username cannot be empty.");
        ui->ERR_MSG_L_P->setStyleSheet("color: red;");
        validLoginUsername = false;
        return;
    }
    validLoginUsername = true;
}


void MainWindow::on_Login_password_textChanged()
{
    ui->ERR_MSG_L_P->setText("");
    if (ui->Login_username->text() == "") {
        ui->ERR_MSG_L_P->setText("Password cannot be empty.");
        ui->ERR_MSG_L_P->setStyleSheet("color: red;");
        validLoginPassword = false;
        return;
    }
    validLoginPassword = true;
}


void MainWindow::on_Login_btn_clicked()
{
    if (validLoginUsername && validLoginPassword) {
        QString username = ui->Login_username->text();
        QString password = ui->Login_password->text();
        if (Task_Manager->userExists(username, password)) {
            QMessageBox message(QMessageBox::Information, "Login Successful", "Login has been successful!",QMessageBox::Ok);
            message.exec();
            user_id = Task_Manager->getUserId(username, password);
            ui->TaskBar->setDisabled(false);
            ui->Search_btn_taskbar->setDisabled(false);
            ui->Search_btn_taskbar->setVisible(true);

            // setting variables for background
            backgroundState = Task_Manager->getTheme(user_id);
            changeFontColor();

            // setting variables for settings page, to be used with pomodoro page as well
            focusTime = Task_Manager->getFocusTime(user_id);
            shortBreak = Task_Manager->getShortTime(user_id);
            longBreak = Task_Manager->getLongTime(user_id);
            time = new QTime(0, focusTime, 0, 0);
            remainingTime = 60 * focusTime;

            // send to home page once login is successful
            MainWindow::on_Home_btn_clicked();

            validLoginUsername = false;
            validLoginPassword = false;
        }
        else {
            ui->ERR_MSG_L_P->setText("Invalid Username or Password.");
            ui->ERR_MSG_L_P->setStyleSheet("color: red;");
        }
    }
    else if (!validLoginUsername) {
        ui->ERR_MSG_L_P->setText("Invalid Login Username. Make sure it is not empty");
        ui->ERR_MSG_L_P->setStyleSheet("color: red;");
    }
    else if (!validLoginPassword) {
        ui->ERR_MSG_L_P->setText("Invalid Login Password. Make sure it is not empty");
        ui->ERR_MSG_L_P->setStyleSheet("color: red;");
    }
}


void MainWindow::on_sendToRegister_btn_clicked()
{
    if (ui->stackedWidget->currentWidget() != ui->Register_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Register.png);");
        ui->stackedWidget->setCurrentWidget(ui->Register_Page);
        ui->Register_username->setText("");
        ui->Register_password->setText("");
        ui->Register_confirmPassword->setText("");
        ui->ERR_MSG_R_P->setText("");
    }
}

// Register Page
void MainWindow::on_Register_username_textChanged()
{
    ui->ERR_MSG_R_P->setText("");
    if (ui->Register_username->text() == "") {
        ui->ERR_MSG_R_P->setText("Username cannot be empty.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterUsername = false;
        return;
    }
    else if (ui->Register_username->text().length() < 4 || ui->Register_username->text().length() > 13) {
        ui->ERR_MSG_R_P->setText("Username cannot be smaller than 4 characters or greater than 13 Characters");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterUsername = false;
        return;
    }
    validRegisterUsername = true;
}


void MainWindow::on_Register_password_textChanged()
{
    bool hasNum = false, hasCap = false, hasLow = false, hasSpecial = false;
    QString pass = ui->Register_password->text();
    ui->ERR_MSG_R_P->setText("");
    if (pass == "") {
        ui->ERR_MSG_R_P->setText("Password cannot be empty.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterPassword = false;
        return;
    }
    else if (pass.length() < 8) {
        ui->ERR_MSG_R_P->setText("Password must atleast have 8 characters.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterPassword = false;
        return;
    }
    for (int i = 0; i < pass.length(); i++) {
        if (pass[i] == ' ') { // no spaces
            ui->ERR_MSG_R_P->setText("Password cannot have spaces.");
            ui->ERR_MSG_R_P->setStyleSheet("color: red;");
            validRegisterPassword = false;
            return;
        }
        else if (pass[i] >= '0' && pass[i] <= '9')  // one digit at least
            hasNum = true;
        else if (pass[i] >= 'a' && pass[i] <= 'z') // one lowercase at least
            hasLow = true;
        else if (pass[i] >= 'A' && pass[i] <= 'Z') // one uppercase at least
            hasCap = true;
        else if (!(pass[i] >= '0' && pass[i] <= '9') && !((pass[i] >= 'a' && pass[i] <= 'z') || (pass[i] >= 'A' && pass[i] <= 'Z'))) // one special character
            hasSpecial = true;
    }
    if (!hasNum) {
        ui->ERR_MSG_R_P->setText("Password must have at least one digit.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterPassword = false;
        return;
    }
    else if (!hasCap) {
        ui->ERR_MSG_R_P->setText("Password must have atleast one Uppercase Letter.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterPassword = false;
        return;
    }
    else if (!hasLow) {
        ui->ERR_MSG_R_P->setText("Password must have atleast one Lowercase Letter.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterPassword = false;
        return;
    }
    else if (!hasSpecial) {
        ui->ERR_MSG_R_P->setText("Password must have atleast one special character such as !, @ or #.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterPassword = false;
        return;
    }
    else
        validRegisterPassword = true;
}


void MainWindow::on_Register_confirmPassword_textChanged()
{
    ui->ERR_MSG_R_P->setText("");
    if (ui->Register_confirmPassword->text() == "") {
        ui->ERR_MSG_R_P->setText("Confirm Password cannot be empty.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterConfirmPassword = false;
        return;
    }
    else if (ui->Register_confirmPassword->text() != ui->Register_password->text()) {
        ui->ERR_MSG_R_P->setText("Passwords dont match.");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        validRegisterConfirmPassword = false;
        return;
    }
    validRegisterConfirmPassword = true;
}


void MainWindow::on_Register_btn_clicked()
{
    if (validRegisterUsername && validRegisterPassword && validRegisterConfirmPassword) {
        QString username = ui->Register_username->text();
        QString password = ui->Register_password->text();
        if(!Task_Manager->userExists(username)) {
            Task_Manager->addUser(username,password);
            QMessageBox message(QMessageBox::Information, "Register Successful", "Registeration has been successful!",QMessageBox::Ok);
            message.exec();
            ui->BackGround->setStyleSheet(
                "background-image: url(:/Background_Images/Sign In Page.png);");
            ui->stackedWidget->setCurrentWidget(ui->Login_Page);
            ui->Login_username->setText("");
            ui->Login_password->setText("");
            ui->ERR_MSG_L_P->setText("");

            validRegisterUsername = false;
            validRegisterPassword = false;
            validRegisterConfirmPassword = false;
        }
        else {
            ui->ERR_MSG_R_P->setText("User already exists with provided credentials.");
            ui->ERR_MSG_R_P->setStyleSheet("color: red;");
        }
    }
    else if (!validRegisterUsername) {
        ui->ERR_MSG_R_P->setText("Username invalid. Make sure it is not empty and follows the format");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
    }
    else if (!validRegisterPassword) {
        ui->ERR_MSG_R_P->setText("Password invalid. Make sure it is not empty and follows the format");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
    }
    else if (!validRegisterConfirmPassword) {
        ui->ERR_MSG_R_P->setText("Confirm Password invalid. Make sure it is not empty and follows the format");
        ui->ERR_MSG_R_P->setStyleSheet("color: red;");
    }
}


void MainWindow::on_sendToLogin_btn_clicked()
{
    if (ui->stackedWidget->currentWidget() != ui->Login_Page) {
        ui->BackGround->setStyleSheet(
            "background-image: url(:/Background_Images/Sign In Page.png);");
        ui->stackedWidget->setCurrentWidget(ui->Login_Page);
        ui->Login_username->setText("");
        ui->Login_password->setText("");
        ui->ERR_MSG_L_P->setText("");
    }
}

// Search page
void MainWindow::on_Search_Name_User_textChanged()
{
    ui->ERR_MSG_Search->setText("");
    if (ui->Search_Name_User->text() == "") {
        ui->ERR_MSG_Search->setText("Search Name cannot be empyty");
        ui->ERR_MSG_Search->setStyleSheet("color: red;");
        validSearchName = false;
        return;
    }
    validSearchName = true;
}


void MainWindow::on_Search_Creation_Date_User_textChanged()
{
    ui->ERR_MSG_Search->setText("");
    if (ui->Search_Creation_Date_User->text() == "") {
        ui->ERR_MSG_Search->setText("Search Creation Date cannot be empty.");
        ui->ERR_MSG_Search->setStyleSheet("color: red;");
        validSearchDate = false;
        return;
    }
    QDate DATE = QDate::fromString(ui->Search_Creation_Date_User->text(), "dd/MM/yyyy");
    if (!DATE.isValid()) {
        ui->ERR_MSG_Search->setText("Invalid Search Creation Date format");
        ui->ERR_MSG_Search->setStyleSheet("color: red;");
        validSearchDate = false;
        return;
    }
    validSearchDate = true;
}


void MainWindow::on_Search_btn_clicked()
{
    if (validSearchName && validSearchDate) {
        int result = layout_Search_create();
        if (result == -1) {
            ui->ERR_MSG_Search->setText("No Task was found with the provided information");
            ui->ERR_MSG_Search->setStyleSheet("color: red;");
            layout_Search_destroy();
        }
    }
    else if (!validSearchName) {
        ui->ERR_MSG_Search->setText("Search Name is invalid. Make sure that it is not empty");
        ui->ERR_MSG_Search->setStyleSheet("color: red;");
    }
    else if (!validSearchDate) {
        ui->ERR_MSG_Search->setText("Search Creation Date is invalid. Make sure that the format is correct");
        ui->ERR_MSG_Search->setStyleSheet("color: red;");
    }
}

int MainWindow::layout_Search_create() {
    int count = 0;
    QString name = ui->Search_Name_User->text();
    QString date = ui->Search_Creation_Date_User->text();
    int* result = Task_Manager->searchTask(name, date, user_id, count);
    if (count == 0 || result == nullptr)
        return -1;
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    ui->scrollArea_Search_Page->setWidget(widget);
    int containerW = 1100, containerH = 60;
    widget->setMaximumSize(containerW, count * (containerH + 20));
    for (int i = 0; i < count; i++) {
        QWidget *container = new QWidget();
        QHBoxLayout *HLayout = new QHBoxLayout(container);

        QPushButton *label = new QPushButton();
        label->setText(Task_Manager->getName(result[i] - 1, user_id));
        label->setStyleSheet("font: 600 14pt Poppins; border: 0;");
        HLayout->addWidget(label);
        HLayout->addStretch(1);
        QWidget *status = new QWidget();
        QHBoxLayout* status_layout = new QHBoxLayout(status);
        status->setFixedWidth(200);
        status->setMaximumHeight(100);
        QLabel *status_text = new QLabel();
        status_layout->addStretch();
        status_layout->addWidget(status_text);
        status_layout->addStretch();
        implementStatus(i, user_id, status, status_text);

        HLayout->addWidget(status);
        container->setLayout(HLayout);
        layout->addWidget(container);
        container->setStyleSheet("margin-bottom: 15px; margin-left: 5px; background-color:transparent; border: 1px solid rgba(74, 72, 72, 127.5); border-radius:15px; margin-right:15px;");
        connect(label, &QPushButton::clicked, this, [this, i, result] {MainWindow::handleSearchButtonClick(result[i] - 1);});
    }
    layout->insertStretch(-1, 1);
    return 1;
}

void MainWindow::layout_Search_destroy() {
    QWidget *widget = ui->scrollArea_Search_Page->widget();
    if (widget) {
        // Delete the layout and all its child widgets
        QLayout *layout = widget->layout();
        if (layout) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                QWidget *widgetItem = item->widget();
                if (widgetItem) {
                    delete widgetItem;
                } else {
                    delete item;
                }
            }
            delete layout;
        }
        delete widget;
    }
}

void MainWindow::handleSearchButtonClick(const int id) {
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Search_" + backgroundState + ".png);");

    ui->scrollArea_Search_Page->setDisabled(true);
    ui->scrollArea_Search_Page->setVisible(false);
    ui->Search_Name_User->setDisabled(true);
    ui->Search_Name_User->setVisible(false);
    ui->Search_Creation_Date_User->setDisabled(true);
    ui->Search_Creation_Date_User->setVisible(false);
    ui->Search_btn->setDisabled(true);
    ui->Search_Name->setDisabled(false);
    ui->Search_Due_Date->setDisabled(false);
    ui->Search_Date_Created->setDisabled(false);
    ui->Search_Status->setDisabled(false);
    ui->Search_Status_text->setDisabled(false);
    ui->Search_Notes->setDisabled(false);
    ui->Search_back_btn->setDisabled(false);

    ui->Search_Name->setText(Task_Manager->getName(id, user_id));
    ui->Search_Due_Date->setText(Task_Manager->getDueDate(id, user_id));
    ui->Search_Date_Created->setText(Task_Manager->getCreationDate(id, user_id));
    implementStatus(id, user_id, ui->Search_Status, ui->Search_Status_text);
    ui->Search_Notes->setText(Task_Manager->getNotes(id, user_id));
}

void MainWindow::on_Search_back_btn_clicked()
{
    ui->BackGround->setStyleSheet(
        "background-image: url(:/Background_Images/Search_Layout_" + backgroundState + ".png);");

    ui->Search_Name->setText("");
    ui->Search_Due_Date->setText("");
    ui->Search_Date_Created->setText("");
    ui->Search_Status->setStyleSheet("");
    ui->Search_Status_text->setText("");
    ui->Search_Notes->setText("");

    ui->scrollArea_Search_Page->setDisabled(false);
    ui->scrollArea_Search_Page->setVisible(true);
    ui->Search_Name_User->setDisabled(false);
    ui->Search_Name_User->setVisible(true);
    ui->Search_Creation_Date_User->setDisabled(false);
    ui->Search_Creation_Date_User->setVisible(true);
    ui->Search_btn->setDisabled(false);
    ui->Search_Name->setDisabled(true);
    ui->Search_Due_Date->setDisabled(true);
    ui->Search_Date_Created->setDisabled(true);
    ui->Search_Status->setDisabled(true);
    ui->Search_Status_text->setDisabled(true);
    ui->Search_Notes->setDisabled(true);
    ui->Search_back_btn->setDisabled(true);
}

// calendar
void MainWindow::on_calendarWidget_currentPageChanged(int year, int month)
{
    // destructing any old labels from previous page if any
    QList<QLabel *> labels = ui->Calendar_Page->findChildren<QLabel *>();
    for (QLabel *label : labels) {
        if (label) {
            if (label != ui->Calendar_text)
                delete label;
        }
    }

    int count = 0;
    int* id_array = nullptr;
    QDate* array = Task_Manager->searchTaskMonth(user_id,month,year, count, id_array);
    int sunday_counter = 0;
    QDate start(year, month - 1, 1);
    while (sunday_counter != 4) {
        int weekday = start.dayOfWeek();
        if (weekday == 1) {
            start = start.addDays(6);
        }
        else if (weekday == 2) {
            start = start.addDays(5);
        }
        else if (weekday == 3) {
            start = start.addDays(4);
        }
        else if (weekday == 4) {
            start = start.addDays(3);
        }
        else if (weekday == 5) {
            start = start.addDays(2);
        }
        else if (weekday == 6) {
            start = start.addDays(1);
        }
        else if (weekday == 7) {
            start = start.addDays(7);
        }
        sunday_counter++;
    }
    QDate currentMonth(year, month, 1);
    int counter = currentMonth.daysInMonth() + start.daysTo(currentMonth);
    const int x_start = 332; //vertical pos start
    const int y_start = 285; // horizontal pos start
    const int x_offset = 153;
    const int y_offset = 101;
    const int y_offset_perLabel = 27;
    int label_count = 0, row = 0, current_col = 0;
    for(int i = 0; i < counter; i++)
    {
        if (i % 7 == 0 && i != 0) {
            row++;
            current_col = 0;
        }
        label_count = 0;
        for(int j = 0; j < count; j++)
        {
            if(start == array[j])
            {
                if (label_count == 3)
                    break;
                QLabel* label = new QLabel(ui->Calendar_Page);
                label->setGeometry(x_start + current_col*x_offset, y_start + row*y_offset + label_count*y_offset_perLabel, 101,20);
                label->setText(Task_Manager->getName(id_array[j] -1, user_id));
                QString status = Task_Manager->getStatus(id_array[j] -1, user_id);
                if (status == "In progress")
                    label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:rgba(0,240,255,181.05); color:rgb(15,76,168); border:transparent; border-radius:10px;");
                else if (status == "Completed") {
                    if (backgroundState == "Light")
                        label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:rgba(51,214,133,100); color:#027A48; border:transparent; border-radius:10px;");
                    else
                        label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:rgba(50,212,132,102); color:#46C791; border:transparent; border-radius:10px;");
                }
                else if (status == "3 Day Mark")
                    label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:rgba(248,22,22,95); color:#FF0000; border:transparent; border-radius:10px;");
                else if (status == "5 Day Mark")
                    label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:#F2BE43; color:#BB6515; border:transparent; border-radius:10px;");
                else if (status == "7 Day Mark")
                    label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:#D4BAE0; color:#B853DB; border:transparent; border-radius:10px;");
                else if (status == "Greater Than 7 Day Mark")
                    label->setStyleSheet("font: 600 8pt Poppins; border: 0; background-color:#1D1C1F; color:white; border:transparent; border-radius:10px;");
                label->setAlignment(Qt::AlignCenter);
                qDebug() << label->pos();
                label->show();
                label_count++;
            }
        }
        start = start.addDays(1);
        current_col++;
    }
}

void MainWindow::on_Calendar_prev_btn_clicked()
{
    int currentMonth = ui->calendarWidget->monthShown();
    int currentYear = ui->calendarWidget->yearShown();
    QDate temp(currentYear, currentMonth,1);
    temp = temp.addMonths(-1);
    ui->calendarWidget->setCurrentPage(temp.year(), temp.month());
    QString date = temp.toString("MMMM, yyyy");
    ui->Calendar_text->setText(date);
}


void MainWindow::on_Calendar_next_btn_clicked()
{
    int currentMonth = ui->calendarWidget->monthShown();
    int currentYear = ui->calendarWidget->yearShown();
    QDate temp(currentYear, currentMonth,1);
    temp = temp.addMonths(1);
    ui->calendarWidget->setCurrentPage(temp.year(), temp.month());
    QString date = temp.toString("MMMM, yyyy");
    ui->Calendar_text->setText(date);
}

