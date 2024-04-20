#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager {
public:
  DbManager(const QString &path);
  ~DbManager();
  bool isOpen() const;
  bool createTasksTable();
  bool createUsersTable();
  bool addTask(const QString &name, const int c_date, const int d_date, const QString &notes);
  bool removePerson(const QString &name);
  bool taskExists(const QString &name, const int c_date, const int d_date) const;
  void printAllPersons() const;
  bool removeAllPersons();

private:
  QSqlDatabase m_db;
};

#endif // DBMANAGER_H
