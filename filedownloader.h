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
        explicit FileDownloader(QUrl url, QObject *parent = nullptr);
        virtual ~FileDownloader();
        QByteArray downloadedData() const;
        QString downloadedFileName() const;

    public slots:
        void cancelDownload();

    private slots:
        void fileDownloaded(QNetworkReply *reply);
        void error(QNetworkReply::NetworkError error);
        void sslErrors(const QList<QSslError> &errors);
        void onReadyRead();

    private:
        QNetworkAccessManager m_manager;
        QNetworkReply *m_reply;
        QByteArray m_downloadedData;
        QString m_fileName;

    signals:
        void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloaded();
};

#endif // FILEDOWNLOADER_H
