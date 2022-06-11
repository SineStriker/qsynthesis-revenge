#include "MainWindow.h"
#include "CMenu.h"

#include <QApplication>
#include <QDebug>
#include <QFile>

static void loadStyleSheet() {
    QFont font("Microsoft YaHei");
    font.setStyleStrategy(QFont::PreferAntialias);
    qApp->setFont(font);

    QFile qss(":/themes/dark-v3.qss");
    if (!qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString data = qss.readAll();
    qApp->setStyleSheet(data);
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
}

MainWindow::~MainWindow() {
}
