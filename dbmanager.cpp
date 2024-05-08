#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <dbmanager.h>
#include <QUuid>

#include <iostream>
#include <cmath>

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

// Task Management

//REQ
bool DbManager::createTasksTable() {
  bool success = true;
    if (!(m_db.tables().contains("tasks"))) {
      QSqlQuery query;
      query.prepare("CREATE TABLE tasks(id INTEGER, name VARCHAR(128), start_date TEXT, "
                    "end_date TEXT, notes VARCHAR(1024), status VARCHAR(50), difference INTEGER, user_id INTEGER NOT NULL, FOREIGN KEY (user_id) REFERENCES users(user_id));");

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
        query.prepare("CREATE TABLE sortedtasks(id INTEGER, name VARCHAR(128), start_date TEXT, "
                      "end_date TEXT, notes VARCHAR(1024), status VARCHAR(50), difference INTEGER, user_id INTEGER NOT NULL, FOREIGN KEY (user_id) REFERENCES users(user_id));");

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
                        const QString &notes, const QString& status, const int difference, const int user_id) {

  int count = 0;
  CountTotalTasks(count, user_id);
  QSqlQuery queryAdd;
  queryAdd.prepare("INSERT INTO tasks(id, name, start_date, end_date, notes, status, difference, user_id) "
                   "VALUES (:id, :name, :start_date, :end_date, :notes, :status, :difference, :user_id)");
  queryAdd.bindValue(":id", count + 1);
  queryAdd.bindValue(":name", name);
  queryAdd.bindValue(":end_date", d_date);
  queryAdd.bindValue(":start_date", c_date);
  queryAdd.bindValue(":notes", notes);
  queryAdd.bindValue(":status", status);
  queryAdd.bindValue(":difference", difference);
  queryAdd.bindValue(":user_id", user_id);

  bool success = false;
  if (queryAdd.exec()) {
    success = true;
  } else {
    qDebug() << "add task failed: " << queryAdd.lastError().text();
    success = false;
  }
  return success;
}

bool DbManager::deleteTask(const int id, const int user_id) {
    QSqlQuery query;
    bool success = false;
    query.prepare("DELETE FROM sortedtasks WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id + 1);
    query.bindValue(":user_id", user_id);
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
    copytomaintable();
    return success;
}

bool DbManager::taskExists(const QString NAME, const QString c_date, const QString d_date, const int user_id, const QString notes) const {
    QString queryStr;
    QSqlQuery query;
    if (notes == "") {
        queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND start_date = :start_date AND end_date = :end_date AND user_id = :user_id";
    }
    else {
        queryStr = "SELECT COUNT(*) FROM tasks WHERE name = :name AND start_date = :start_date AND end_date = :end_date AND notes = :notes AND user_id = :user_id";
    }
    query.prepare(queryStr);
    query.bindValue(":name", NAME);
    query.bindValue(":start_date", c_date);
    query.bindValue(":end_date", d_date);
    if (notes != "") {query.bindValue(":notes", notes);}
    query.bindValue(":user_id", user_id);
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


void DbManager::CountTotalTasks(int& count, const int user_id){
    QSqlQuery query;
    query.prepare("SELECT id FROM sortedtasks where user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Error executing Total Tasks query:" << query.lastError().text();
        return;
    }
    while (query.next()) {
        count++;
    }
}

//REQ
QString DbManager::getName(const int id, const int user_id){
    QSqlQuery query;
    query.prepare("SELECT name FROM sortedtasks WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":user_id", user_id);
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

QString DbManager::getDueDate(const int id, const int user_id){
    QSqlQuery query;
    query.prepare("SELECT end_date FROM sortedtasks WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":user_id", user_id);
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

QString DbManager::getCreationDate(const int id, const int user_id){
    QSqlQuery query;
    query.prepare("SELECT start_date FROM sortedtasks WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":user_id", user_id);
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


QString DbManager::getNotes(const int id, const int user_id){
    QSqlQuery query;
    query.prepare("SELECT notes FROM sortedtasks WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":user_id", user_id);
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

QString DbManager::getStatus(const int id, const int user_id){
    QSqlQuery query;
    query.prepare("SELECT status FROM sortedtasks WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":user_id", user_id);
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

int DbManager::getDifference(const int id, const int user_id) {
    QSqlQuery query;
    query.prepare("SELECT difference FROM sortedtasks where id = :id AND user_id = :user_id");
    query.bindValue(":id", id + 1);
    query.bindValue(":user_id", user_id);
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

void DbManager::setName(const int id, const int user_id, const QString name) {
    QSqlQuery query;
    query.prepare("UPDATE sortedtasks SET name = :name WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":name", name);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Name updated successfully.";
    }
    copytomaintable();
}

void DbManager::setDueDate(const int id, const int user_id, const QString date){
    QSqlQuery query;
    query.prepare("UPDATE sortedtasks SET end_date = :date WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":date", date);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Date updated successfully.";
    }
    copytomaintable();
}

void DbManager::setNotes(const int id, const int user_id, const QString notes){
    QSqlQuery query;
    query.prepare("UPDATE sortedtasks SET notes = :notes WHERE id = :id AND user_id = :user_id");
    query.bindValue(":id", id+1);
    query.bindValue(":notes", notes);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Notes updated successfully.";
    }
    copytomaintable();
}

void DbManager::setStatus(const int id, const int user_id, const QString status) {
    QSqlQuery query;
    query.prepare("UPDATE sortedtasks SET status = :status WHERE id = :id AND user_id = :user_id");
    query.bindValue(":status", status);
    query.bindValue(":id", id +1);
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Status updated successfully.";
    }
    copytomaintable();
}

void DbManager::sortTasks(){
    dropSortedTable();
    createSortedTable();
    QString queryString = "SELECT name, start_date, end_date, notes, status, difference, user_id FROM tasks ORDER BY difference";
    QSqlQuery query(queryString);
    if (!query.exec()) {
        qDebug() << "Error fetching tasks:" << query.lastError().text();
        return;
    }
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO sortedtasks (id, name, start_date, end_date, notes, status, difference, user_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    // Iterate through sorted results from tasks and insert into sortedtasks
    while (query.next()) {
        int index = 0;
        int user_id = query.value(6).toInt();
        CountTotalTasks(index,user_id);
        // Bind values from the sorted query results
        insertQuery.addBindValue(index + 1); // id
        insertQuery.addBindValue(query.value(0)); // name
        insertQuery.addBindValue(query.value(1)); // start_date
        insertQuery.addBindValue(query.value(2)); // end_date
        insertQuery.addBindValue(query.value(3)); // notes
        insertQuery.addBindValue(query.value(4)); // status
        insertQuery.addBindValue(query.value(5)); // difference
        insertQuery.addBindValue(query.value(6)); // user_id

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting sorted task:" << insertQuery.lastError().text();
        }
    }
    qDebug() << "Sorted tasks copied to 'sortedtasks' table.";
    copytomaintable();
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

void DbManager::copytomaintable(){
    dropTasksTable();
    createTasksTable();
    QString queryString = "SELECT id, name, start_date, end_date, notes, status, difference, user_id FROM sortedtasks;";
    QSqlQuery query(queryString);
    if (!query.exec()) {
        qDebug() << "Error fetching tasks:" << query.lastError().text();
        return;
    }
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO tasks (id, name, start_date, end_date, notes, status, difference, user_id) VALUES (? , ?, ?, ?, ?, ?, ?, ?)");

    // Iterate through sorted results and insert into tasks
    while (query.next()) {
        // Bind values from the sorted query results
        insertQuery.addBindValue(query.value(0)); // id
        insertQuery.addBindValue(query.value(1)); // name
        insertQuery.addBindValue(query.value(2)); // start_date
        insertQuery.addBindValue(query.value(3)); // end_date
        insertQuery.addBindValue(query.value(4)); // notes
        insertQuery.addBindValue(query.value(5)); // status
        insertQuery.addBindValue(query.value(6)); // difference
        insertQuery.addBindValue(query.value(7)); // user_id

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting sorted task:" << insertQuery.lastError().text();
        }
    }
    qDebug() << "tasks copied to 'tasks' table.";
}

int* DbManager::searchTask(const QString name, const QString c_date, const int user_id, int& count) {
    QDate c_date_date = QDate::fromString(c_date, "dd/MM/yyyy");
    QDate c_date_date_less = c_date_date.addDays(-1);
    QDate c_date_date_more = c_date_date.addDays(1);
    // wild card operator % is used to check if name and c_date is present anywhere regardless of start and end characters
    QSqlQuery countQuery;
    countQuery.prepare("SELECT COUNT(*) FROM tasks WHERE name LIKE '%' || :name || '%' AND (start_date = :start_date OR start_date = :date1 OR start_date = :date2) AND user_id = :user_id");
    countQuery.bindValue(":name", name);
    countQuery.bindValue(":start_date", c_date);
    countQuery.bindValue(":date1", c_date_date_less.toString("dd/MM/yyyy"));
    countQuery.bindValue(":date2", c_date_date_more.toString("dd/MM/yyyy"));
    countQuery.bindValue(":user_id", user_id);
    if(!countQuery.exec()) {
        qDebug() << "Failed to execute query:" << countQuery.lastError().text();
        return nullptr; // Return nullptr if query execution fails
    }
    if(countQuery.next()) {
        count = countQuery.value(0).toInt();
    } else {
        qDebug() << "No tasks found with the provided name and creation date.";
        return nullptr; // Return nullptr if no task is found
    }

    int* result = new int[count];
    int index =0;
    QSqlQuery query;
    query.prepare("SELECT id FROM tasks WHERE name LIKE '%' || :name || '%' AND (start_date = :start_date OR start_date = :date1 OR start_date = :date2) AND user_id = :user_id");
    query.bindValue(":name", name);
    query.bindValue(":start_date", c_date);
    query.bindValue(":date1", c_date_date_less.toString("dd/MM/yyyy"));
    query.bindValue(":date2", c_date_date_more.toString("dd/MM/yyyy"));
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return nullptr; // Return nullptr if query execution fails
    }
    while(query.next()) {
        result[index] = query.value(0).toInt();
        index++;
    }
    return result;
}

QDate* DbManager::searchTaskMonth(const int user_id, const int month, const int year, int& count, int*& id_arr){

    QString string;
    if (month < 10)
       string = "0" + QString::fromStdString(std::to_string(month)) + "/" + QString::fromStdString(std::to_string(year));
    else
       string = QString::fromStdString(std::to_string(month)) + "/" + QString::fromStdString(std::to_string(year));
    qDebug() << string;
    QSqlQuery query;
    query.prepare("SELECT end_date FROM tasks WHERE end_date LIKE '%' ||:string AND user_id = :user_id");
    query.bindValue(":string", string);
    query.bindValue(":user_id", user_id);

    QSqlQuery countQuery;
    countQuery.prepare("SELECT COUNT(*) FROM tasks WHERE end_date LIKE '%' ||:string AND user_id = :user_id");
    countQuery.bindValue(":string", string);
    countQuery.bindValue(":user_id", user_id);

    QSqlQuery idQuery;
    idQuery.prepare("SELECT id FROM tasks WHERE end_date LIKE '%' ||:string AND user_id = :user_id");
    qDebug() <<  QString::fromStdString(std::to_string(month)) << " 2: " << QString::fromStdString(std::to_string(year));
    idQuery.bindValue(":string", string);
    idQuery.bindValue(":user_id", user_id);

    if(!countQuery.exec()){
        qDebug() << "searchTaskMonth count failed";
    }
    if(countQuery.next()) {
        count = countQuery.value(0).toInt();
    }
    QDate* date_array = new QDate[count];
    id_arr = new int[count];

    if(!query.exec()){
        qDebug() << "seachTaskMonth search failed";
    }
    int index = 0;
    while(query.next()){
        QDate date = QDate::fromString(query.value(0).toString(), "dd/MM/yyyy");
        date_array[index] = date;
        index++;
    }
    if(!idQuery.exec()){
        qDebug() << "searchtaskMonth id get failed";
        qDebug() << idQuery.lastError().text();
    }
    int i = 0;
    while(idQuery.next()){
        id_arr[i++] = idQuery.value(0).toInt();
    }
    return date_array;
}

// User Management
QString DbManager::encryptPassword(const QString& password) {
    std::string temp = password.toStdString();
    for (int i = 0; i < password.length(); i++) {
        temp[i] += 7;
    }
    return QString::fromStdString(temp);
}

QString DbManager::decryptPassword(QString& password) {
    std::string temp = password.toStdString();
    for (int i = 0; password.length(); i++) {
        temp[i] -= 7;
    }
    password = QString::fromStdString(temp);
    return password;
}

bool DbManager::createUsersTable() {
    bool success = true;
    if (!(m_db.tables().contains("users"))) {
        QSqlQuery query;
        query.prepare("CREATE TABLE users(user_id INTEGER PRIMARY KEY, username VARCHAR(128) NOT NULL UNIQUE, password VARCHAR(128) NOT NULL, theme TEXT, hoursWorked FLOAT, \
                      tasksDone INTEGER, focusTime INTEGER, shortTime INTEGER, longTime INTEGER);");

            if (!query.exec()) {
            qDebug() << "Couldn't create the table 'users'";
            success = false;
        } else {
            qDebug() << "Table created successfully";
        }
    }
    return success;
}

bool DbManager::addUser(const QString& username, const QString& password, const QString& theme, const float& hoursWorked, const int& tasksDone, const int& focusTime, const int& shortTime, const int& longTime) {
    QString encrypted_password = encryptPassword(password);
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO users(username, password, theme, hoursWorked, tasksDone, focusTime, shortTime, longTime) "
                     "VALUES (:username, :password, :theme, :hoursWorked, :tasksDone, :focusTime, :shortTime, :longTime)");
    queryAdd.bindValue(":username", username);
    queryAdd.bindValue(":password", encrypted_password);
    queryAdd.bindValue(":theme", theme);
    queryAdd.bindValue(":hoursWorked", hoursWorked);
    queryAdd.bindValue(":tasksDone", tasksDone);
    queryAdd.bindValue(":focusTime", focusTime);
    queryAdd.bindValue(":shortTime", shortTime);
    queryAdd.bindValue(":longTime", longTime);

    bool success = false;
    if (queryAdd.exec()) {
        success = true;
    } else {
        qDebug() << "Add user failed: " << queryAdd.lastError().text();
        success = false;
    }
    return success;
}

bool DbManager::userExists(const QString& username, const QString& password) {
    QSqlQuery query;
    if (password == "")
        query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    else {
        QString encrypted_password = encryptPassword(password);
        query.prepare("SELECT COUNT(*) FROM users WHERE username = :username AND password = :encrypted_password");
        query.bindValue(":encrypted_password", encrypted_password);
    }
    query.bindValue(":username", username);

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

int DbManager::getUserId(const QString username, const QString password) {
    QString encrypted_password = encryptPassword(password);
    QSqlQuery query;
    query.prepare("SELECT user_id FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", encrypted_password);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        int user_id = query.value(0).toInt();
        return user_id;
    } else {
        qDebug() << "No User found with the provided username and password.";
        return -1; // Return -1 if no user is found
    }
}

QString DbManager::getUserName(const int user_id)
{
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return ""; // Return empty string if query execution fails
    }
    if(query.next()) {
        QString username = query.value(0).toString();
        return username;
    } else {
        qDebug() << "No username found with the provided id.";
        return ""; // Return an empty QString if no task is found
    }
}

QString DbManager::getTheme(const int user_id) {
    QSqlQuery query;
    query.prepare("SELECT theme FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return ""; // Return empty string if query execution fails
    }
    if(query.next()) {
        QString theme = query.value(0).toString();
        return theme;
    } else {
        qDebug() << "No User found with the provided id.";
        return ""; // Return an empty QString if no task is found
    }
}

float DbManager::getHoursWorked(const int user_id)
{
    QSqlQuery query;
    query.prepare("SELECT hoursWorked FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        float hoursWorked = query.value(0).toFloat();
        return hoursWorked;
    } else {
        qDebug() << "No User found with the provided id.";
        return -1; // Return -1 if no user is found
    }
}

int DbManager::getTasksDone(const int user_id)
{
    QSqlQuery query;
    query.prepare("SELECT tasksDone FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        int tasksDone = query.value(0).toInt();
        return tasksDone;
    } else {
        qDebug() << "No User found with the provided id.";
        return -1; // Return -1 if no user is found
    }
}

int DbManager::getFocusTime (const int user_id) {
    QSqlQuery query;
    query.prepare("SELECT focusTime FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        int focusTime = query.value(0).toInt();
        return focusTime;
    } else {
        qDebug() << "No User found with the provided id.";
        return -1; // Return -1 if no user is found
    }
}

int DbManager::getShortTime (const int user_id) {
    QSqlQuery query;
    query.prepare("SELECT shortTime FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        int shortTime = query.value(0).toInt();
        return shortTime;
    } else {
        qDebug() << "No User found with the provided id.";
        return -1; // Return -1 if no user is found
    }
}

int DbManager::getLongTime (const int user_id) {
    QSqlQuery query;
    query.prepare("SELECT longTime FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);
    if(!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1; // Return -1 if query execution fails
    }
    if(query.next()) {
        int longTime = query.value(0).toInt();
        return longTime;
    } else {
        qDebug() << "No User found with the provided id.";
        return -1; // Return -1 if no user is found
    }
}

void DbManager::setTheme(const int user_id, const QString theme)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET theme = :theme WHERE user_id = :user_id");
    query.bindValue(":theme", theme);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Theme updated successfully.";
    }
}

void DbManager::setHoursWorked(const int user_id, float hoursWorked)
{
    double roundedValue = std::round(hoursWorked * std::pow(10, 2)) / std::pow(10, 2);
    QSqlQuery query;
    query.prepare("UPDATE users SET hoursWorked = :hoursWorked WHERE user_id = :user_id");
    query.bindValue(":hoursWorked", roundedValue);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Hours Worked updated successfully.";
    }
}

void DbManager::setTasksDone(const int user_id, int tasksDone)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET tasksDone = :tasksDone WHERE user_id = :user_id");
    query.bindValue(":tasksDone", tasksDone);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Tasks Done updated successfully.";
    }
}

void DbManager::setfocusTime(const int user_id, int focusTime)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET focusTime = :focusTime WHERE user_id = :user_id");
    query.bindValue(":focusTime", focusTime);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Focus Time updated successfully.";
    }
}

void DbManager::setshortTime(const int user_id, int shortTime)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET shortTime = :shortTime WHERE user_id = :user_id");
    query.bindValue(":shortTime", shortTime);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Short Time updated successfully.";
    }
}

void DbManager::setlongTime(const int user_id, int longTime)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET longTime = :longTime WHERE user_id = :user_id");
    query.bindValue(":longTime", longTime);
    query.bindValue(":user_id", user_id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    } else {
        qDebug() << "Long Time updated successfully.";
    }
}







