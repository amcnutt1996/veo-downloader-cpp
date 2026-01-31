#include <iostream>
#include <QApplication>
#include "main_window.h"
#include "web_scraper.h"

int main(int argc, char *argv[]) {

    const QApplication vidDownloader(argc, argv);

    main_window primary_win;
    primary_win.show();

    // test call of the video link grabber from VEO website.
    std::string video_url = web_scraper::get_video_url("https://app.veo.co/matches/20260122-training-jan-22-2026-f0194918/");
    return QApplication::exec();
    return 0;
}