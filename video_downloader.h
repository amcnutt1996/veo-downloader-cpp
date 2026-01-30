//
// Created by Gregory McNutt on 1/29/26.
//

#ifndef VIDEODOWNLOADERGUI_VIDEO_DOWNLOADER_H
#define VIDEODOWNLOADERGUI_VIDEO_DOWNLOADER_H
#include <qnetworkreply.h>
#include <Qdir>



class video_downloader {
    static void onNetworkReply(QNetworkReply *reply);
    void networkRequest() const;

private:
    QNetworkAccessManager *networkManager;

private slots:
    void onDownloadStarted();
    void onDownloadCancelled();
    void onSetDownloadDirectory();
    void onDownloadReadyRead();

};


#endif //VIDEODOWNLOADERGUI_VIDEO_DOWNLOADER_H