//
// Created by Gregory McNutt on 1/29/26.
//

#include "video_downloader.h"
#include "main_window.h"
#include <iostream>
#include <QFileDialog>
#include <QNetworkReply>
#include <QProgressBar>
#include <string>
#include <QTimer>
#include <sys/socket.h>

#include "ui_main_window.h"


void video_downloader::downloadVideoFile(const std::string &video_url, const QDir &filePath, QProgressBar *progressBar) {
    //create file to save to and get the string version of the filepath set by user.
    QString fullSavePath = filePath.filePath("Video.mp4");
    qDebug() << "saving to: " << fullSavePath;

    newFile = new QFile(fullSavePath);

    if (!newFile->open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file for writing...";
        delete newFile;
        newFile = nullptr;
        return;
    }
    m_progressBar = progressBar;
    //create new object for connection requests
    connectToUrl = new QNetworkAccessManager(this);

    //QNetworkAccessManager connectToURL(new QNetworkAccessManager);
    //convert the string URL into a QUrl for connection.
    const QUrl new_url((video_url.c_str()));
    //Connect to the specified URL
    const QNetworkRequest connectionRequest(new_url);

    reply = connectToUrl->get(connectionRequest);

    connect(reply, &QNetworkReply::readyRead, this,&video_downloader::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &video_downloader::onDownloadProgress);
    connect(reply, &QNetworkReply::finished, this, &video_downloader::onDownloadFinished);

}


void video_downloader::stopDownload() const {
    if (reply) {
        reply->abort();
        reply->close();
        reply->deleteLater();
    }
    if (newFile) {
        newFile->remove();
        delete newFile;
    }
    qDebug() << "User Cancelled Download";
}

void video_downloader::onDownloadProgress(qint64 bytesRead, qint64 totalBytes) const {
    if (totalBytes > 0) {
        const int percent = (bytesRead * 100) / totalBytes;
        m_progressBar->setValue(percent);
    }
}

void video_downloader::onReadyRead() {
    if (reply && newFile) {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (statusCode.isValid() && statusCode.toInt() != 200) {
            newFile->close();
            newFile->remove();
            delete newFile;
            newFile = nullptr;
            QTimer::singleShot(0, reply, &QNetworkReply::abort);
            return;
        }
        newFile->write(reply->readAll());
    }
}

void video_downloader::onDownloadFinished() {
    if (newFile) {
        qDebug() << "file saved to :" << newFile->filesystemFileName();
        newFile->close();
        newFile->deleteLater();
        newFile = nullptr;
    }
    if (reply) {
        reply->deleteLater();
        reply = nullptr;
    }
    emit downloadCompleted();
    qDebug() << "Download Finished. Cleaning complete.";
}

// std::string fileNamer(std::string &original_url){
//
//     return ;
// }
