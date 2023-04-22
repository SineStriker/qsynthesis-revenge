#include "MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_treeWidget = new QsApi::AceTreeWidget();
    setCentralWidget(m_treeWidget);

    auto model = m_treeWidget->model();

    m_buffer.setBuffer(&m_data);
    m_buffer.open(QIODevice::WriteOnly);

    model->startRecord(&m_buffer);
    connect(model, &QsApi::AceTreeModel::stepChanged, this, &MainWindow::_q_modelStepChanged);

    auto listItem1 = new QsApi::AceTreeItem("Track");
    auto setItem1 = new QsApi::AceTreeItem("Note");

    auto rootItem = new QsApi::AceTreeItem("Root");
    rootItem->setProperty("text", QLatin1String("YQ's god"));
    rootItem->insertRows(0, {listItem1});
    rootItem->insertNode(setItem1);

    model->setRootItem(rootItem);

    listItem1->setProperty("text", QLatin1String("aaa"));
    setItem1->setProperty("text", QLatin1String("bbb"));
}

MainWindow::~MainWindow() {
}

void MainWindow::_q_modelStepChanged(int step) {
    qDebug() << "step" << step;
    qDebug() << m_data;
}
