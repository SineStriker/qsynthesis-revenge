#include "MainWindow.h"
#include "CMenu.h"
#include "Utils/QCssAnalyzer.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QScreen>

static void loadStyleSheet() {
    QCssAnalyzer qss(":/themes/dark-v3.qss");
    double ratio = qApp->primaryScreen()->logicalDotsPerInch() / 96.0;
    qss.setRatio(0.8 * ratio);
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(qss.readAndApply());
        qss.close();
    }
}

MainWindow::MainWindow(QWidget *parent) : BasicWindow(parent) {
    loadStyleSheet();

    auto bar = menuBar();
    auto menu1 = new CMenu("Menu 1");
    auto menu2 = new CMenu("Menu 2");

    menu1->addActions({new QAction("Action 1"), new QAction("Action 2")});
    menu2->addActions({new QAction("Action 3"), new QAction("Action 4")});

    bar->addMenu(menu1);
    bar->addMenu(menu2);

    resize(1280, 720);

    setWindowTitle(qApp->applicationDisplayName());
}

MainWindow::~MainWindow() {
}
