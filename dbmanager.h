#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager {
public:
  DbManager(const QString &path);
  ~DbManager();

  bool createTasksTable();
  bool dropTasksTable();
  bool createUsersTable();
  bool createSortedTable();
  bool dropSortedTable();
  bool addTask(const QString &name, const QString& c_date, const QString& d_date, const QString &notes, const int difference, const QString& status = "In progress");
  bool deleteTask(const int id);
  bool modifyTask(const int id, QString name, QString d_date, QString c_date, QString notes, QString status);
  void CountTotalTasks(int& count);
  bool taskExists(const QString name, const QString c_date, const QString d_date, const QString notes = "") const;
  QString getName(const int id);
  QString getDueDate(const int id);
  QString getCreationDate(const int id);
  QString getNotes(const int id);
  QString getStatus(const int id);
  int getDifference(const int id);
  void setName(const int id, const QString name);
  void setDueDate(const int id, const QString date);
  void setNotes(const int id, const QString notes);
  void setStatus(const int id, const QString status);
  void sortTasks();
  void copytomain();
private:
  QSqlDatabase m_db;
};

#endif // DBMANAGER_H
