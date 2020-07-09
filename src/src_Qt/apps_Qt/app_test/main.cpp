/**
 * @file main.cpp
 * @brief test Qt application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#include <QApplication>
#include <QDebug>
#include "MainWindow.h"




int main(int argc, char *argv[]) {
    QApplication App(argc, argv);

    gui::MainWindow applicatioGUI;

    applicatioGUI.show();

    return App.exec();
}




