//
// Created by Gregory McNutt on 1/29/26.
//

#ifndef VIDEODOWNLOADERGUI_VIDEO_DOWNLOADER_H
#define VIDEODOWNLOADERGUI_VIDEO_DOWNLOADER_H

#include <QNetworkAccessManager>
#include <QFile>
#include <qprogressbar.h>


class video_downloader : public QObject{
    Q_OBJECT
public:
    void downloadVideoFile(const std::string &video_url, const QDir &filePath, QProgressBar *progressBar);
    void stopDownload();
    QNetworkReply *reply = nullptr;
    QFile *newFile = nullptr;
private:
    QNetworkAccessManager *connectToUrl = nullptr;
    QProgressBar *m_progressBar = nullptr;

signals:
    void downloadCompleted();

public slots:
    void onDownloadProgress(qint64 bytesRead, qint64 totalBytes) const;
    void onReadyRead();
    void onDownloadFinished();
};


#endif //VIDEODOWNLOADERGUI_VIDEO_DOWNLOADER_H