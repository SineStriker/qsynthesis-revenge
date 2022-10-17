#include "MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    tabs = new QScrollableTabWidget();
    setCentralWidget(tabs);
}

MainWindow::~MainWindow() {
}
