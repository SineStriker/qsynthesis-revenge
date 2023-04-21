#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_treeWidget = new QsApi::AceTreeWidget();
    setCentralWidget(m_treeWidget);

    auto rootItem = new QsApi::AceTreeItem("Root");
    rootItem->setProperty("text", "YQ's God");
    rootItem->insertRows(0, {new QsApi::AceTreeItem("Note")});

    m_treeWidget->model()->setRootItem(rootItem);
}

MainWindow::~MainWindow() {
}