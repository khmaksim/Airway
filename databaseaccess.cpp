#include "databaseaccess.h"
#include <QSqlDatabase>
#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QSqlRecord>
#include <QDateTime>
#include <QApplication>
#include <QSettings>
#include <QMessageBox>

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
}

DatabaseAccess* DatabaseAccess::getInstance()
{
    static DatabaseAccess instance;
    instance.connect();
    return &instance;
}

//DatabaseAccess* DatabaseAccess::getInstance()
//{
//    if(databaseAccess == 0)
//       databaseAccess = new DatabaseAccess;
//    return databaseAccess;
//}

void DatabaseAccess::readSettings()
{
    QSettings settings;

    settings.beginGroup("database");
    fileNameDatabase = settings.value("file").toString();
    settings.endGroup();
}

void DatabaseAccess::connect()
{
    readSettings();

    db = QSqlDatabase::addDatabase("QSQLITE", "airway");
    if (!QFile(fileNameDatabase).exists()) {
        qDebug() << "File database is not found!";
    }

    db.setDatabaseName(fileNameDatabase);
    if (db.open()) {
        QMessageLogger(nullptr, 0, nullptr).info("Connect database");
        QMessageLogger(nullptr, 0, nullptr).debug() << "file database" << fileNameDatabase;
    }
    else {
        QMessageLogger(nullptr, 0, nullptr).warning("Failed to connect to the database");
        QMessageLogger(nullptr, 0, nullptr).debug() << db.lastError().text();
        qDebug() << db.lastError().text();
    }

    db.exec("PRAGMA FOREIGN_KEYS=ON");       //set support for foreign keys
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
