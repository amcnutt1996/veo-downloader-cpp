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
#include <QMessageBox>

// #include "cmake-build-release-intel/videoDownloaderGUI_autogen/include/ui_main_window.h"

// TODO: link validation for non-veo links input into the url line.
// TODO: naming feature for finalized file post download.
// TODO: Implement log feature for what the app is doing in background (clear out on close)

main_window::main_window(QWidget *parent)
    // initialize the main window and set up the main UI elements.
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    //set default directory to same one as app default
    defaultSavePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    ui->setupUi(this);

    // initialize the two external objects we're using
    m_video_downloader = new video_downloader();
    m_web_scraper = new web_scraper();

    //clear out the progress bar on init
    ui->downloadProgress->setValue(0);

    //connects the button clicks to the socket functions to interact with the classes from the other files.
    connect(ui->selectDirBtn, &QPushButton::clicked, this, &main_window::selectFilePath);
    connect(ui->downloadBtn, &QPushButton::clicked, this, &main_window::startDownload);
    connect(ui->stopDownloadBtn, &QPushButton::clicked, this, &main_window::halt_download);
    connect(ui->userInputURL, &QLineEdit::returnPressed, this, &main_window::startDownload);
    m_escShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    m_escShortcut->setEnabled(false);
    connect(m_escShortcut, &QShortcut::activated, this, &main_window::halt_download);
    // this connects the video downloader to the main window's download completed function so that it can modify
    // UI elements when the download completes.
    connect(m_video_downloader, &video_downloader::downloadCompleted, this, &main_window::downloadCompleted);


}

void main_window::downloadCompleted() const {
    // socket: happens when the download is completed from the function in video_downloader.
    ui->userInputURL->clear();
    ui->downloadProgress->setValue(0);
    ui->statusBar->showMessage("Download Complete");
    setDownloadingState(false);
}

void main_window::halt_download() const {
    // socket: allows the download to be halted when user clicks the button
    setDownloadingState(false);
    m_video_downloader->stopDownload();
    ui->downloadProgress->setValue(0);
    ui->statusBar->showMessage("User Cancelled Download...");
}


void main_window::startDownload() const {
    // socket: allows the buttonpress to start the download.
    // gathers required information to be input into the function from the Qt main window to pass to the external functions.

        if (linkValidation(ui->userInputURL->text()) == true) {

        const std::string stringURL = ui->userInputURL->text().toStdString();
        // takes original string and finds the source video URL.
        const std::string scraped_string = web_scraper::get_video_url(stringURL);

        // locks UI so user cant fuck with it while downloading.
        setDownloadingState(true);

        // passes the new data into the function within video downloader
        m_video_downloader->downloadVideoFile(scraped_string, defaultSavePath, ui->downloadProgress);

        ui->statusBar->showMessage("Downloading...");
        qDebug() << "Starting download...";

    } else {
        QMessageBox infoBox;
        infoBox.setIcon(QMessageBox::Information);
        infoBox.setWindowTitle("Veo Downloader");
        infoBox.setText("Please use a valid app.veo.co link from the VEO video site.");
        infoBox.exec();
        ui->userInputURL->clear();
    }

}

void main_window::selectFilePath() {
    //helper function to assist with selection of the chosen filepath when the user clicks the button.
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
        //if the file path is valid, it sets that location to the new file path.
        defaultSavePath = filepath;
    } else {
        qDebug() << "User cancelled the selection";
        ui->statusBar->showMessage("User cancelled directory selection.");
    }
}

void main_window::setDownloadingState(const bool isDownloading) const {
    // helper to set the state of if the user is downloading or not.
    // If we are not downloading something we can enable all inputs.
    bool inputsEnabled = !isDownloading; // <- opposite of isDownloading

    ui->downloadBtn->setEnabled(inputsEnabled);
    ui->userInputURL->setEnabled(inputsEnabled);
    ui->selectDirBtn->setEnabled(inputsEnabled);

    //make sure the stop button is still enabled when it's downloading.
    ui->stopDownloadBtn->setEnabled(isDownloading);
    m_escShortcut->setEnabled(true);
}


bool main_window::linkValidation(const QString &initialURL) {
    if (initialURL.contains("app.veo.co")) {
        return true;
    } else {
        return false;
    }
}


main_window::~main_window() {
    delete ui;
}
