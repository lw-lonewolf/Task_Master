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

//REQ
bool DbManager::createTasksTable() {
  bool success = true;
    if (!(m_db.tables().contains("tasks"))) {
      QSqlQuery query;
      query.prepare("CREATE TABLE tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(128), start_date TEXT, "
                    "end_date TEXT, notes VARCHAR(1024), status VARCHAR(50));");

      if (!query.exec()) {
        qDebug() << "Couldn't create the table 'tasks'";
        success = false;
      } else {
        qDebug() << "Table created successfully";
      }
    }
  return success;
}

//REQ
bool DbManager::addTask(const QString &name, const QString c_date, const QString d_date,
                        const QString &notes, const QString& status) {

  QSqlQuery queryAdd;
  queryAdd.prepare("INSERT INTO tasks(name, start_date, end_date, notes, status) "
                   "VALUES (:name, :start_date, :end_date, :notes, :status)");
  queryAdd.bindValue(":name", name);
  queryAdd.bindValue(":end_date", d_date);
  queryAdd.bindValue(":start_date", c_date);
  queryAdd.bindValue(":notes", notes);
  queryAdd.bindValue(":status", status);
bool success = false;
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

//REQ
// bool DbManager::taskExists(const int id, const QString NAME, const QString c_date, const QString d_date) const {
//     QString queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND start_date = :start_date AND end_date = :end_date AND id <> :id";
//     QSqlQuery query;
//     query.prepare(queryStr);
//     query.bindValue(":name", NAME);
//     query.bindValue(":id", id);
//     query.bindValue(":start_date", c_date);
//     query.bindValue(":end_date", d_date);
//     if (!query.exec()) {
//         qDebug() << "Error executing query:" << query.lastError().text();
//         return false;
//     }
//     query.next();
//     int count = query.value(0).toInt();
//     if (count > 0) {
//         return true;
//     }
//     return false;
// }

bool DbManager::taskExists(const QString NAME, const QString c_date, const QString d_date, const QString notes) const {
    QString queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND start_date = :start_date AND end_date = :end_date AND notes = :notes";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":name", NAME);
    query.bindValue(":start_date", c_date);
    query.bindValue(":end_date", d_date);
    query.bindValue(":notes", notes);
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return false;
    }
    query.next();
    int count = query.value(0).toInt();
    if (count > 0) {
        return true;
    }
    return false;
}


//REQ
void DbManager::CountTotalTasks(int& count){
    QSqlQuery query("SELECT id FROM tasks", m_db);
    while (query.next()) {
        count++;
    }
}

//REQ
QString DbManager::getName(const int id){
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHERE id = :id");
    query.bindValue(":id", id+1);
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

QString DbManager::getDueDate(const int id){
    QSqlQuery query;
    query.prepare("SELECT end_date FROM tasks WHERE id = :id");
    query.bindValue(":id", id+1);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString date = query.value(0).toString();
        return date;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}

QString DbManager::getCreationDate(const int id){
    QSqlQuery query;
    query.prepare("SELECT start_date FROM tasks WHERE id = :id");
    query.bindValue(":id", id+1);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString date = query.value(0).toString();
        return date;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}


QString DbManager::getNotes(const int id){
    QSqlQuery query;
    query.prepare("SELECT notes FROM tasks WHERE id = :id");
    query.bindValue(":id", id+1);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString date = query.value(0).toString();
        return date;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}

QString DbManager::getStatus(const int id){
    QSqlQuery query;
    query.prepare("SELECT status FROM tasks WHERE id = :id");
    query.bindValue(":id", id+1);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString date = query.value(0).toString();
        return date;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}

void DbManager::setName(const int id, const QString name) {
    QSqlQuery query;
    query.prepare("UPDATE tasks SET name = :name WHERE id = :id");
    query.bindValue(":id", id+1);
    query.bindValue(":name", name);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Name updated successfully.";
    }
}

void DbManager::setDueDate(const int id, const QString date){
    QSqlQuery query;
    query.prepare("UPDATE tasks SET end_date = :date WHERE id = :id");
    query.bindValue(":id", id+1);
    query.bindValue(":date", date);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Date updated successfully.";
    }
}
void DbManager::setNotes(const int id, const QString notes){
    QSqlQuery query;
    query.prepare("UPDATE tasks SET notes = :notes WHERE id = :id");
    query.bindValue(":id", id+1);
    query.bindValue(":notes", notes);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Notes updated successfully.";
    }
}

