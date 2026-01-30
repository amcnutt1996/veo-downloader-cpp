cmake_minimum_required(VERSION 4.1)
project(videoDownloaderGUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(videoDownloaderGUI main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        web_scraper.cpp
        web_scraper.h
        video_downloader.cpp
        video_downloader.h)

target_link_libraries(videoDownloaderGUI
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        CURL::libcurl
        LibXml2::LibXml2
)



