//
// Created by Gregory McNutt on 1/29/26.
//

#include "video_downloader.h"
#include <iostream>
#include <QFileDialog>
#include <QNetworkReply>
#include <QProgressBar>
#include <string>
#include <QTimer>
#include "main_window.h"


void video_downloader::downloadVideoFile(const std::string &video_url, const QDir &filePath, QProgressBar *progressBar) {
    // get the new filename to use later when naming the file.
    const QString new_file_name = fileNameFromUrl(main_window::user_url);
    qDebug() << "New Filename: " << new_file_name;
    //create file to save to and get the string version of the filepath set by user.
    const QString fullSavePath = filePath.filePath( new_file_name + ".part");
    qDebug() << "saving to: " << fullSavePath;

    //create the new file
    newFile = new QFile(fullSavePath);

    //make sure the new file is valid and writable so we can download to it.
    if (!newFile->open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file for writing...";
        delete newFile;
        newFile = nullptr;
        return;
    }
    qDebug() << "opened file: " << newFile->fileName();

    // creating a pointer to the progressbar that gets passed so we can update it when file is downloading.
    m_progressBar = progressBar;
    //create new object for connection requests
    connectToUrl = new QNetworkAccessManager(this);

    //convert the string URL into a QUrl for connection.
    const QUrl new_url(QString::fromStdString(video_url)); // Safer conversion
    //Connect to the specified URL
    const QNetworkRequest connectionRequest(new_url);

    //creates the reply based on the connection's requested URL.
    reply = connectToUrl->get(connectionRequest);
    qDebug() << "Connected to " << connectionRequest.url();

    // connects the functions to the network reply states depending on what is's doing.
    connect(reply, &QNetworkReply::readyRead, this,&video_downloader::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &video_downloader::onDownloadProgress);
    connect(reply, &QNetworkReply::finished, this, &video_downloader::onDownloadFinished);

}


void video_downloader::stopDownload() {
    if (reply) {
        (void)reply->disconnect();
        
        reply->abort();
        reply->close();
        reply->deleteLater();
        reply = nullptr;
    }
    if (newFile) {
        newFile->remove();
        delete newFile;
        newFile = nullptr;
    }
    qDebug() << "User Cancelled Download";
}

void video_downloader::onDownloadProgress(const qint64 bytesRead, const qint64 totalBytes) const {
    if (totalBytes > 0) {
        const int percent = static_cast<int>((bytesRead * 100) / totalBytes);
        m_progressBar->setValue(percent);
    }
}

void video_downloader::onReadyRead() {
    if (reply && newFile) {
        const QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (statusCode.isValid() && statusCode.toInt() != 200) {
            qDebug() << "Invalid Status Code: status code = " << statusCode;
            newFile->close();
            newFile->remove();
            delete newFile;
            newFile = nullptr;
            qDebug() << "Deleted new file because the download errored out";
            QTimer::singleShot(0, reply, &QNetworkReply::abort); //adds the ability to add the next download to the queue one after another.
            return;
        }
        newFile->write(reply->readAll());
    }
}

void video_downloader::onDownloadFinished() {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Download failed with error: " << reply->errorString();
    } else {
        qDebug() << "Download completed successfully.";
    }


    if (newFile) {
        qDebug() << "file saved to :" << newFile ->filesystemFileName();
        newFile->close();

        //renames the file from .part to .mp4 so the user can open it once its finished downloading.
        const QString currentPath = newFile->fileName();
        QString newPath = currentPath;
        newPath.replace(".part", ".mp4");

        if (newFile->rename(newPath)) {
            qDebug() << "file successfully renamed";
        } else {
            qDebug() << "Rename failed:" << newFile->errorString();
        }

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

//std::string video_url = web_scraper::get_video_url("https://app.veo.co/matches/20260122-training-jan-22-2026-f0194918/");

QString video_downloader::fileNameFromUrl(const std::string &original_url){
    const auto start_index = original_url.find("matches/");
    std::string new_filename = original_url.substr(start_index + 8, -1);
    new_filename.pop_back();
    QString qstr_file_name = new_filename.c_str();
    return qstr_file_name;
}
