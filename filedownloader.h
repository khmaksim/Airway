#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
        Q_OBJECT
    public:
        explicit FileDownloader(QObject *parent = nullptr);
        virtual ~FileDownloader();
        QByteArray downloadedData() const;

    private slots:
        void fileDownloaded(QNetworkReply *reply);

    private:
        QNetworkAccessManager m_manager;
        QByteArray m_downloadedData;

    signals:
        void downloaded();
};

#endif // FILEDOWNLOADER_H
