//
// Created by Gregory McNutt on 1/29/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_main_window.h" resolved

#include "main_window.h"
#include "ui_main_window.h"
#include "web_scraper.h"
#include <iostream>
#include <QString>

main_window::main_window(QWidget *parent) : QWidget(parent), ui(new Ui::main_window) {
    ui->setupUi(this);
    std::cout << "Initializing Application" << std::endl;
}



main_window::~main_window() {
    delete ui;
}