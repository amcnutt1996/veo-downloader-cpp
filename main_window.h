//
// Created by Gregory McNutt on 1/29/26.
//

#ifndef VIDEODOWNLOADERGUI_MAIN_WINDOW_H
#define VIDEODOWNLOADERGUI_MAIN_WINDOW_H

#include <QDir>
#include <QShortcut>
#include <QMainWindow>
#include "video_downloader.h"
#include "web_scraper.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class main_window;
}

QT_END_NAMESPACE

class main_window : public QMainWindow {
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window() override;

public slots:
    void selectFilePath();
    void startDownload() const;
    void downloadCompleted() const;
    void halt_download() const;

private:
    Ui::main_window *ui;
    QDir defaultSavePath;
    video_downloader *m_video_downloader = nullptr;
    web_scraper *m_web_scraper = nullptr;
    QShortcut *m_escShortcut = nullptr; // Hotkey Object
    void setDownloadingState(bool isDownloading) const;

    static bool linkValidation(const QString &initialURL);
};


#endif //VIDEODOWNLOADERGUI_MAIN_WINDOW_H