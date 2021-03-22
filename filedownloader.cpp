#include "filedownloader.h"
#include <QDebug>
#include <QCoreApplication>

FileDownloader::FileDownloader(QUrl url, QObject *parent) : QObject(parent)
{
    connect(&m_manager, &QNetworkAccessManager::finished, this, &FileDownloader::fileDownloaded);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setRawHeader("User-Agent", "AirwayApp 1.0");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/pdf");
    qDebug() << url;
//    QNetworkReply *reply = m_manager.get(request);

//    QEventLoop loop;
    m_reply = m_manager.get(request);

    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &FileDownloader::error);
    connect(m_reply, &QNetworkReply::sslErrors, this, &FileDownloader::sslErrors);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &FileDownloader::updateDownloadProgress);
    connect(m_reply, &QNetworkReply::readyRead, this, &FileDownloader::onReadyRead);
//    loop.exec();
}

FileDownloader::~FileDownloader() {}

void FileDownloader::onReadyRead()
{
    m_downloadedData.append(m_reply->readAll());
}

void FileDownloader::fileDownloaded(QNetworkReply *reply)
{
    m_fileName = reply->url().fileName();
    //emit a signal
    reply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_downloadedData;
}

QString FileDownloader::downloadedFileName() const
{
    return m_fileName;
}

void FileDownloader::error(QNetworkReply::NetworkError error)
{
    qDebug() << error;
}

void FileDownloader::sslErrors(const QList<QSslError> &errors)
{
    qDebug() << errors;
}

void FileDownloader::cancelDownload()
{
    if (m_reply)
        m_reply->abort();
}
