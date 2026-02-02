cmake_minimum_required(VERSION 3.20)
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

set(APP_ICON "${CMAKE_SOURCE_DIR}/veodownloader.icns")

set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION TRUE)

add_executable(videoDownloaderGUI
        main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        web_scraper.cpp
        web_scraper.h
        video_downloader.cpp
        video_downloader.h
        veodownloader.icns
        ${APP_ICON}
)

# Configure the Bundle properties
set_target_properties(videoDownloaderGUI PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_ICON_FILE "veodownloader.icns"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.gregorymcnutt.videodownloader"
    MACOSX_BUNDLE_BUNDLE_NAME "VEO Downloader"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
)

target_link_libraries(videoDownloaderGUI
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        CURL::libcurl
        LibXml2::LibXml2
)
