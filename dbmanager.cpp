#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <dbmanager.h>

DbManager::DbManager(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

  if (!m_db.open()) {
    qDebug() << "Error: connection with database fail";
  } else {
    qDebug() << "Database: connection ok";
  }
}

DbManager::~DbManager() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool DbManager::isOpen() const { return m_db.isOpen(); }

bool DbManager::createTasksTable() {
  bool success = true;
    if (!(m_db.tables().contains("tasks"))) {
      QSqlQuery query;
      query.prepare("CREATE TABLE tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(128), start_date INT, "
                    "end_date INT, notes VARCHAR(1024), status VARCHAR(50));");

      if (!query.exec()) {
        qDebug() << "Couldn't create the table 'tasks'";
        success = false;
      } else {
        qDebug() << "Table created successfully";
      }
    }
  return success;
}

bool DbManager::addTask(const QString &name, const int c_date, const int d_date,
                        const QString &notes, const QString& status) {
  bool success = false;
  QSqlQuery queryAdd;
  queryAdd.prepare("INSERT INTO tasks(name, start_date, end_date, notes, status) "
                   "VALUES (:name, :start_date, :end_date, :notes, :status)");
  queryAdd.bindValue(":name", name);
  queryAdd.bindValue(":end_date", d_date);
  queryAdd.bindValue(":start_date", c_date);
  queryAdd.bindValue(":notes", notes);
  queryAdd.bindValue(":status", status);
  if (queryAdd.exec()) {
    success = true;
  } else {
    qDebug() << "add person failed: " << queryAdd.lastError();
    success = false;
  }
  return success;
}

// bool DbManager::removePerson(const QString &name) {
//   bool success = false;

//  if (personExists(name)) {
//    QSqlQuery queryDelete;
//    queryDelete.prepare("DELETE FROM people WHERE name = (:name)");
//    queryDelete.bindValue(":name", name);
//    success = queryDelete.exec();

//    if (!success) {
//      qDebug() << "remove person failed: " << queryDelete.lastError();
//    }
//  } else {
//    qDebug() << "remove person failed: person doesnt exist";
//  }

//   return success;
// }

void DbManager::printAllPersons() const {
  qDebug() << "Persons in db:";
  QSqlQuery query("SELECT * FROM people");
  int idName = query.record().indexOf("name");
  while (query.next()) {
    QString name = query.value(idName).toString();
    qDebug() << "===" << name;
  }
}

bool DbManager::taskExists(const QString &NAME, const int c_date, const int d_date) const {
  QString queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND ((start_date <= :end_date AND end_date >= :start_date) OR (start_date >= :start_date AND end_date <= :end_date))";
  QSqlQuery query;
  query.prepare(queryStr);
  query.bindValue(":name", NAME);
  query.bindValue(":start_date", c_date);
  query.bindValue(":end_date", d_date);
  if (!query.exec()) {
      qDebug() << "Error executing query:" << query.lastError().text();
      return false;
  }
  query.next();
  int count = query.value(0).toInt();
  if (count > 0){
      return true;
  }
  return false;
}

bool DbManager::removeAllPersons() {
  bool success = false;

  QSqlQuery removeQuery;
  removeQuery.prepare("DELETE FROM people");

  if (removeQuery.exec()) {
    success = true;
  } else {
    qDebug() << "remove all persons failed: " << removeQuery.lastError();
  }

  return success;
}

void DbManager::CountTotalTasks(int& count){
    QSqlQuery query("SELECT id FROM tasks", m_db);
    while (query.next()) {
        count++;
    }
}

QString DbManager::getName(const int id){
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString name = query.value(0).toString();
        return name;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}

