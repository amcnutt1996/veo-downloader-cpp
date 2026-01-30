//
// Created by Gregory McNutt on 1/29/26.
//

#ifndef VIDEODOWNLOADERGUI_MAIN_WINDOW_H
#define VIDEODOWNLOADERGUI_MAIN_WINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class main_window;
}

QT_END_NAMESPACE

class main_window : public QWidget {
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);

    static void resetProgressBar();

    ~main_window() override;

private:
    Ui::main_window *ui;
};


#endif //VIDEODOWNLOADERGUI_MAIN_WINDOW_H