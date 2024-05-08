#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QDate>

class DbManager {
public:
  DbManager(const QString &path);
  ~DbManager();

  // functions for Task Management
  bool createTasksTable();
  bool dropTasksTable();
  bool createSortedTable();
  bool dropSortedTable();
  bool addTask(const QString &name, const QString& c_date, const QString& d_date, const QString &notes, const QString& status, const int difference, const int user_id);
  bool deleteTask(const int id, const int user_id);
  void CountTotalTasks(int& count, const int user_id);
  bool taskExists(const QString name, const QString c_date, const QString d_date, const int user_id, const QString notes = "") const;
  QString getName(const int id, const int user_id);
  QString getDueDate(const int id, const int user_id);
  QString getCreationDate(const int id, const int user_id);
  QString getNotes(const int id, const int user_id);
  QString getStatus(const int id, const int user_id);
  int getDifference(const int id, const int user_id);
  void setName(const int id, const int user_id, const QString name);
  void setDueDate(const int id, const int user_id, const QString date);
  void setNotes(const int id, const int user_id, const QString notes);
  void setStatus(const int id, const int user_id, const QString status);
  void sortTasks();
  void copytomaintable();
  int* searchTask(const QString name, const QString c_date, const int user_id,int& count);
  QDate* searchTaskMonth(const int user_id, const int month, const int year, int& count, int*& id_arr);

  // functions for User Management
  QString encryptPassword(const QString& password);
  QString decryptPassword(QString& password);
  bool createUsersTable();
  bool addUser(const QString& username, const QString& password, const QString& theme = "Light", const float& hoursWorked = 0.0, const int& tasksDone = 0, const int& focusTime = 25, const int& shortTime = 5, const int& longTime = 15);
  bool userExists(const QString& username, const QString& password = "");
  int getUserId(const QString username, const QString password);
  QString getUserName(const int user_id);
  QString getTheme(const int user_id);
  float getHoursWorked(const int user_id);
  int getTasksDone(const int user_id);
  int getFocusTime (const int user_id);
  int getShortTime (const int user_id);
  int getLongTime (const int user_id);
  void setTheme(const int user_id, const QString theme);
  void setHoursWorked(const int user_id, float hoursWorked);
  void setTasksDone(const int user_id, int tasksDone);
  void setfocusTime(const int user_id, int focusTime);
  void setshortTime(const int user_id, int shortTime);
  void setlongTime(const int user_id, int longTime);
private:
  QSqlDatabase m_db;
};

#endif // DBMANAGER_H
