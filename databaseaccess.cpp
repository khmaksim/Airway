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
    readSettings();

    db = QSqlDatabase::addDatabase("QSQLITE", "airway");
    if (!QFile(fileNameDatabase).exists()) {
        qDebug() << "File database is not found!";
    }

    db.setDatabaseName(fileNameDatabase);
    if (db.open()) {
        QMessageLogger(0, 0, 0).info("Connect database");
        QMessageLogger(0, 0, 0).debug() << "file database" << fileNameDatabase;
    }
    else {
        QMessageLogger(0, 0, 0).warning("Failed to connect to the database");
        QMessageLogger(0, 0, 0).debug() << db.lastError().text();
        qDebug() << db.lastError().text();
    }

    db.exec("PRAGMA FOREIGN_KEYS=ON");       //set support for foreign keys
}

DatabaseAccess* DatabaseAccess::getInstance()
{
    static DatabaseAccess instance;
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

QVector<Record> DatabaseAccess::getAirways()
{
    QSqlQuery query(db);
    QVector<Record> airways = QVector<Record>();

    query.exec("SELECT code FROM airway_name ORDER BY code");
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

//    query.exec("SELECT ap.code_airway, p.code, p.name, p.code_state, p.lat, p.lon "
//               "FROM airway_point ap LEFT OUTER JOIN point p ON p.code = ap.code_point");
    query.exec("SELECT ap.code_airway, p.code, p.name, p.code_state, p.lat, p.lon "
               "FROM airway_point ap, point p WHERE p.code = ap.code_point ORDER BY ap.code_airway, ap.\"order\"");

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
