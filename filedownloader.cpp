#include "filedownloader.h"

FileDownloader::FileDownloader(QObject *parent) : QObject(parent)
{

    connect(&m_manager, &QNetworkAccessManager::finished, this, &FileDownloader::fileDownloaded);
}

FileDownloader::~FileDownloader() {}

void FileDownloader::fileDownloaded(QNetworkReply *reply)
{
    m_downloadedData = reply->readAll();
    //emit a signal
    reply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_downloadedData;
}

