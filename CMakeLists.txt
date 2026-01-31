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

# Define variables for your resource files
set(APP_ICON "${CMAKE_SOURCE_DIR}/veodownloader.icns")

# Tell CMake that the icon file should be copied to the 'Resources' folder inside the bundle
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(videoDownloaderGUI MACOSX_BUNDLE
        main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        web_scraper.cpp
        web_scraper.h
        video_downloader.cpp
        video_downloader.h
        ${APP_ICON}       # Add icon to source list
)

# Configure the Bundle properties
set_target_properties(videoDownloaderGUI PROPERTIES
    # This tells macOS which file in Resources is the icon
    MACOSX_BUNDLE_ICON_FILE "veodownloader.icns"
    # These properties fill in the auto-generated Info.plist
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.gregorymcnutt.videodownloader"
    MACOSX_BUNDLE_BUNDLE_NAME "VEO Downloader"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
)

target_link_libraries(videoDownloaderGUI
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        CURL::libcurl
        LibXml2::LibXml2
)
