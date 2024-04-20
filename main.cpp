#include "mainwindow.h"
#include "dbmanager.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Database initialisation
    DbManager Task_Manager(QDir::currentPath() + "/tasks.db");
    // Sending in the db object as a parameter for the mainwindow's constructor
    MainWindow w(Task_Manager);
    w.show();
    return a.exec();
}
