#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    m_g2pWidget = new G2pWidget(this);
    m_tableWidget = new TableWidget(this);
    m_configWidget = new ConfigWidget(this);

    m_tabWidget->addTab(m_configWidget, "配置");
    m_tabWidget->addTab(m_g2pWidget, "歌词");
    m_tabWidget->addTab(m_tableWidget, "用户字典");
    resize(800, 600);
}

MainWindow::~MainWindow() {
}
