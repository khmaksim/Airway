#include "databaseaccess.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlRecord>
#include <QDateTime>
#include <QApplication>
#include <QDebug>

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
}

DatabaseAccess* DatabaseAccess::getInstance()
{
    static DatabaseAccess instance;
//    instance.connect();               // SQLite
    return &instance;
}

//DatabaseAccess* DatabaseAccess::getInstance()
//{
//    if(databaseAccess == 0)
//       databaseAccess = new DatabaseAccess;
//    return databaseAccess;
//}


void DatabaseAccess::initConnectDatabase(const QString &host, int port, const QString &nameDatabase, const QString &user, const QString &password)
{
    this->host = host;
    this->port = port;
    this->nameDatabase = nameDatabase;
    this->user = user;
    this->password = password;
}

bool DatabaseAccess::connect(const QVariant &configConectDatabase)
{
    QMap<QString, QVariant> config = configConectDatabase.toMap();

    if (config.isEmpty())
        db = QSqlDatabase::addDatabase("QPSQL");
    else {
        db = QSqlDatabase::addDatabase("QPSQL", QString("test"));
        host = config.value("host").toString();
        port = config.value("port").toInt();
        nameDatabase = config.value("nameDatabase").toString();
        user = config.value("user").toString();
        password = config.value("password").toString();
    }

    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(nameDatabase);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open()) {
        if (!host.isEmpty() && !nameDatabase.isEmpty() && !user.isEmpty() && !password.isEmpty())
            emit notificationConnected(db.lastError().text());
        return false;
    }
    emit connected();

    // remove test connect to database
    if (db.connectionName().contains("test"))
        QSqlDatabase::removeDatabase(QString("test"));

    return true;
}

QVector<Record> DatabaseAccess::getAirways()
{
    QSqlQuery query(db);
    QVector<Record> airways = QVector<Record>();

    query.exec("SELECT code_ru FROM airway ORDER BY code_ru");
    while (query.next()) {
        Record record;
        QSqlRecord sqlRecord = query.record();

        int col = 0;
        while (col < sqlRecord.count()){
            record.insert(col, query.value(col));
            col++;
        }
        airways.append(record);
    }
    return airways;
}

QVector<Record> DatabaseAccess::getPoints()
{
    QSqlQuery query(db);
    QVector<Record> points = QVector<Record>();

    query.exec("SELECT ap.code_airway, p.name_ru, p.lat, p.lon, ap.magnetic_track_angle_forward, "
               "ap.magnetic_track_angle_back, ap.minimum_altitude, ap.width, "
               "ap.direction_trains_forward, ap.direction_trains_back, ap.upper_limit, ap.lower_limit, "
               "p.name_ru "
               "FROM airway_point ap, point p "
               "WHERE p.name_ru = ap.code_point ORDER BY ap.code_airway, ap.\"order\"");

    if (query.lastError().isValid())
        qDebug() << query.lastError().text() << query.lastQuery();

    while (query.next()) {
        Record record;
        QSqlRecord sqlRecord = query.record();
        int col = 0;
        while (col < sqlRecord.count()){
            record.insert(col, query.value(col));
            col++;
        }
        points.append(record);
    }

    return points;
}

bool DatabaseAccess::isConnected() const
{
    return QSqlDatabase::database().isOpen();
}
