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

        void initConnectDatabase(const QString &host, int port, const QString &nameDatabase, const QString &user, const QString &password);
        bool isConnected() const;
        bool connect(const QVariant &configConectDatabase = QVariant());

    private:
        DatabaseAccess(QObject *parent = nullptr);
        DatabaseAccess(const DatabaseAccess&);
        DatabaseAccess& operator =(const DatabaseAccess);
//        static DatabaseAccess *databaseAccess;

        QSqlDatabase db;
        QString host;
        QString nameDatabase;
        QString user;
        QString password;
        int port;

    signals:
        void connected();
        void notificationConnected(const QString);
};

#endif // DATABASEACCESS_H
