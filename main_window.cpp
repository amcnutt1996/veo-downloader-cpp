//
// Created by Gregory McNutt on 1/29/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_main_window.h" resolved

#include "main_window.h"
#include "ui_main_window.h"
#include "video_downloader.h"
#include "web_scraper.h"
#include <iostream>
#include <QDir>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QStatusBar>
#include <QStandardPaths>

#include "video_downloader.h"


main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    defaultSavePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation); //set default directory to same one as app default
    ui->setupUi(this);
    std::cout << "Initializing Application" << std::endl;
    m_video_downloader = new video_downloader();
    m_web_scraper = new web_scraper();

    ui->downloadProgress->setValue(0);
    connect(ui->selectDirBtn, &QPushButton::clicked, this, &main_window::selectFilePath);
    connect(ui->downloadBtn, &QPushButton::clicked, this, &main_window::startDownload);

    connect(m_video_downloader, &video_downloader::downloadCompleted, this, &main_window::downloadCompleted);
}

void main_window::downloadCompleted() const {
    // socket: happens when the download is completed from the function in video_downloader.
    ui->userInputURL->clear();
    ui->downloadProgress->setValue(0);
    ui->statusBar->showMessage("Download Complete");
}


void main_window::startDownload() const {
    // socket: allows the buttonpress to start the download.
    // gathers required input from the Qt main window to pass to the external functions.
    const std::string stringURL = ui->userInputURL->text().toStdString();

    std::string scraped_string = web_scraper::get_video_url(stringURL);

    m_video_downloader->downloadVideoFile(scraped_string, defaultSavePath, ui->downloadProgress);
    ui->statusBar->showMessage("Download Started");
    qDebug() << "Starting download...";

}

void main_window::selectFilePath() {
    const QString filepath = QFileDialog::getExistingDirectory( //returns full string of filepath
    this,
    //dialog title
    "Select Directory",
    //starting directory
    "/home",
    //options for dialogue
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (!filepath.isEmpty()) {
        qDebug() << "User selected: " << filepath;
        defaultSavePath = filepath;
    } else {
        qDebug() << "User cancelled the selection";
    }
}

main_window::~main_window() {
    delete ui;
}
