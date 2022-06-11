#include "MainWindow.h"
#include "CMenu.h"

#include "Utils/QCssAnalyzer.h"
#include "QScrollableTabWidget.h"

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

    auto tabs = new QScrollableTabWidget();
    setCentralWidget(tabs);
    tabs->addTab(new QLabel("1"), "tab-1");
    tabs->addTab(new QLabel("2"), "tab-2");
    tabs->addTab(new QLabel("3"), "tab-3");
    tabs->addTab(new QLabel("11"), "tab-111111");
    tabs->addTab(new QLabel("22"), "tab-222222");
    tabs->addTab(new QLabel("33"), "tab-33333333");
    tabs->addTab(new QLabel("44"), "tab-4444");
    tabs->addTab(new QLabel("55"), "tab-555555");
    tabs->addTab(new QLabel("66"), "tab-666");
    tabs->addTab(new QLabel("77"), "tab-77777777");
    tabs->addTab(new QLabel("88"), "tab-8888888888");

    resize(1280, 720);

    setWindowTitle(qApp->applicationDisplayName());
}

MainWindow::~MainWindow() {
}
