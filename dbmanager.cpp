#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <dbmanager.h>
#include <QUuid>

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
      query.prepare("CREATE TABLE tasks(id INTEGER PRIMARY KEY, name VARCHAR(128), start_date TEXT, "
                    "end_date TEXT, notes VARCHAR(1024), status VARCHAR(50), difference INTEGER);");

      if (!query.exec()) {
        qDebug() << "Couldn't create the table 'tasks'";
        success = false;
      } else {
        qDebug() << "Table created successfully";
      }
    }
  return success;
}

bool DbManager::createSortedTable() {
    bool success = true;
    if (!(m_db.tables().contains("sortedtasks"))) {
        QSqlQuery query;
        query.prepare("CREATE TABLE sortedtasks(id INTEGER PRIMARY KEY, name VARCHAR(128), start_date TEXT, "
                      "end_date TEXT, notes VARCHAR(1024), status VARCHAR(50), difference INTEGER);");

        if (!query.exec()) {
            qDebug() << "Couldn't create the table 'tasks'";
            success = false;
        } else {
            qDebug() << "Table created successfully";
        }
    }
    return success;
}

bool DbManager::dropSortedTable() {
    QSqlQuery query;
    query.prepare("DROP TABLE IF EXISTS sortedtasks");
    bool success = query.exec();
    if (!success) {
        qDebug() << "Error dropping sortedtasks table:" << query.lastError().text();
    } else {
        qDebug() << "sortedtasks table dropped successfully.";
    }
    return success;
}
//REQ
bool DbManager::addTask(const QString &name, const QString& c_date, const QString& d_date,
                        const QString &notes, const int difference, const QString& status) {

  QSqlQuery queryAdd;
  queryAdd.prepare("INSERT INTO tasks(name, start_date, end_date, notes, status, difference) "
                   "VALUES (:name, :start_date, :end_date, :notes, :status, :difference)");
  queryAdd.bindValue(":name", name);
  queryAdd.bindValue(":end_date", d_date);
  queryAdd.bindValue(":start_date", c_date);
  queryAdd.bindValue(":notes", notes);
  queryAdd.bindValue(":status", status);
  queryAdd.bindValue(":difference", difference);


  bool success = false;
  if (queryAdd.exec()) {
    success = true;
  } else {
    qDebug() << "add task failed: " << queryAdd.lastError().text();
    success = false;
  }
  return success;
}

bool DbManager::deleteTask(const int id) {
    QSqlQuery query;
    bool success = false;
    query.prepare("DELETE FROM sortedTasks WHERE id = :id");
    query.bindValue(":id", id + 1);
    if(!query.exec()) {
        qDebug() << "Delete Task failed: " << query.lastError().text();
        success = false;
    }
    else {
        success = true;
        // Updating ids of all task rows in front of the deleted task to ensure that id's remain correct.
        QSqlQuery idquery;
        idquery.prepare("UPDATE sortedTasks SET id = id - 1 WHERE id > :id");
        idquery.bindValue(":id", id + 1);
        if(!idquery.exec()) {
            qDebug() << "ID Update failed: " << idquery.lastError().text();
            success = false;
        }
    }
    copytomain();
    return success;
}

bool DbManager::taskExists(const QString NAME, const QString c_date, const QString d_date, const QString notes) const {
    QString queryStr;
    QSqlQuery query;
    if (notes == "") {
        queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND start_date = :start_date AND end_date = :end_date";
    }
    else {
        queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND start_date = :start_date AND end_date = :end_date AND notes = :notes";
    }
    query.prepare(queryStr);
    query.bindValue(":name", NAME);
    query.bindValue(":start_date", c_date);
    query.bindValue(":end_date", d_date);
    if (notes != "") {query.bindValue(":notes", notes);}
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
    QSqlQuery query("SELECT id FROM sortedTasks", m_db);
    while (query.next()) {
        count++;
    }
}

//REQ
QString DbManager::getName(const int id){
    QSqlQuery query;
    query.prepare("SELECT name FROM sortedTasks WHERE id = :id");
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
    query.prepare("SELECT end_date FROM sortedTasks WHERE id = :id");
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
    query.prepare("SELECT start_date FROM sortedTasks WHERE id = :id");
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
    query.prepare("SELECT notes FROM sortedTasks WHERE id = :id");
    query.bindValue(":id", id+1);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString notes = query.value(0).toString();
        return notes;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}

QString DbManager::getStatus(const int id){
    QSqlQuery query;
    query.prepare("SELECT status FROM sortedTasks WHERE id = :id");
    query.bindValue(":id", id+1);
    if(!query.exec()){
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QString(); // Return an empty QString if query execution fails
    }

    if(query.next()) {
        QString status = query.value(0).toString();
        return status;
    } else {
        qDebug() << "No task found with the provided id.";
        return QString(); // Return an empty QString if no task is found
    }
}

int DbManager::getDifference(const int id) {
    QSqlQuery query;
    query.prepare("SELECT difference FROM sortedTasks where id = :id");
    query.bindValue(":id", id + 1);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        int difference = query.value(0).toInt();
        return difference;
    } else {
        qDebug() << "No task found with the provided id.";
        return -1; // Return -1 if no task is found
    }
}

void DbManager::setName(const int id, const QString name) {
    QSqlQuery query;
    query.prepare("UPDATE sortedTasks SET name = :name WHERE id = :id");
    query.bindValue(":id", id+1);
    query.bindValue(":name", name);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Name updated successfully.";
    }
    copytomain();
}

void DbManager::setDueDate(const int id, const QString date){
    QSqlQuery query;
    query.prepare("UPDATE sortedTasks SET end_date = :date WHERE id = :id");
    query.bindValue(":id", id+1);
    query.bindValue(":date", date);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Date updated successfully.";
    }
    copytomain();
}

void DbManager::setNotes(const int id, const QString notes){
    QSqlQuery query;
    query.prepare("UPDATE sortedTasks SET notes = :notes WHERE id = :id");
    query.bindValue(":id", id+1);
    query.bindValue(":notes", notes);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Notes updated successfully.";
    }
    copytomain();
}

void DbManager::setStatus(const int id, const QString status) {
    QSqlQuery query;
    query.prepare("UPDATE sortedTasks SET status = :status WHERE id = :id");
    query.bindValue(":status", status);
    query.bindValue(":id", id +1);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Status updated successfully.";
    }
    copytomain();
}

void DbManager::sortTasks(){
    dropSortedTable();
    createSortedTable();
    QString queryString = "SELECT name, start_date, end_date, notes, status, difference FROM tasks ORDER BY difference";
    QSqlQuery query(queryString);
    if (!query.exec()) {
        qDebug() << "Error fetching tasks:" << query.lastError().text();
        return;
    }
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO sortedtasks (name, start_date, end_date, notes, status, difference) VALUES (?, ?, ?, ?, ?, ?)");

    // 3. Iterate through sorted results and insert into sortedtasks
    while (query.next()) {
        // Bind values from the sorted query results
        insertQuery.addBindValue(query.value(0)); // name
        insertQuery.addBindValue(query.value(1)); // start_date
        insertQuery.addBindValue(query.value(2)); // end_date
        insertQuery.addBindValue(query.value(3)); // notes
        insertQuery.addBindValue(query.value(4)); // status
        insertQuery.addBindValue(query.value(5)); // difference

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting sorted task:" << insertQuery.lastError().text();
        }
    }
    qDebug() << "Sorted tasks copied to 'sortedtasks' table.";
}

bool DbManager::dropTasksTable(){
    QSqlQuery query;
    query.prepare("DROP TABLE IF EXISTS tasks");
    bool success = query.exec();
    if (!success) {
        qDebug() << "Error dropping tasks table:" << query.lastError().text();
    } else {
        qDebug() << "tasks table dropped successfully.";
    }
    return success;
}

void DbManager::copytomain(){
    dropTasksTable();
    createTasksTable();
    QString queryString = "SELECT name, start_date, end_date, notes, status, difference FROM sortedTasks;";
    QSqlQuery query(queryString);
    if (!query.exec()) {
        qDebug() << "Error fetching tasks:" << query.lastError().text();
        return;
    }
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO tasks (name, start_date, end_date, notes, status, difference) VALUES (?, ?, ?, ?, ?, ?)");

    // 3. Iterate through sorted results and insert into sortedtasks
    while (query.next()) {
        // Bind values from the sorted query results
        insertQuery.addBindValue(query.value(0)); // name
        insertQuery.addBindValue(query.value(1)); // start_date
        insertQuery.addBindValue(query.value(2)); // end_date
        insertQuery.addBindValue(query.value(3)); // notes
        insertQuery.addBindValue(query.value(4)); // status
        insertQuery.addBindValue(query.value(5)); // difference

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting sorted task:" << insertQuery.lastError().text();
        }
    }
    qDebug() << "tasks copied to 'tasks' table.";
}
