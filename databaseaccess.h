#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVariant>

typedef QMap<int, QVariant> Record;
class DatabaseAccess : public QObject
{
        Q_OBJECT
    public:
        static DatabaseAccess* getInstance();

        QVector<Record> getAirways();
        QVector<Record> getPoints();

    private:
        DatabaseAccess(QObject *parent = nullptr);
        DatabaseAccess(const DatabaseAccess&);
        DatabaseAccess& operator =(const DatabaseAccess);
//        static DatabaseAccess *databaseAccess;

        QSqlDatabase db;
        QString fileNameDatabase;

        void connect();
        void readSettings();

//    signals:
//        void updatedTags();
};

#endif // DATABASEACCESS_H
