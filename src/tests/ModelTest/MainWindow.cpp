#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_treeWidget = new AceTreeWidget();
    setCentralWidget(m_treeWidget);

    auto model = m_treeWidget->model();

    QString filename = qApp->applicationDirPath() + "/1.dat";

    //    m_buffer.setBuffer(&m_data);
    //    m_buffer.open(QIODevice::WriteOnly);
    //    model->startRecord(&m_buffer);

    AceTreeItem *root = nullptr;

    {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            auto model1 = AceTreeModel::recover(file.readAll());
            file.close();
            if (model1) {
                qDebug() << "Recover success";
                root = model1->reset();
                delete model1;
            }
        }
    }

    m_file.setFileName(filename);
    m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    model->startLogging(&m_file);

    if (root) {
        model->setRootItem(root);
    } else {
        auto listItem1 = new AceTreeItem("Track");
        auto setItem1 = new AceTreeItem("Note");

        auto rootItem = new AceTreeItem("Root");
        rootItem->setProperty("text", QLatin1String("YQ's god"));
        rootItem->insertRows(0, {listItem1});
        rootItem->insertNode(setItem1);

        listItem1->setProperty("text", QLatin1String("aaa"));
        setItem1->setProperty("text", QLatin1String("bbb"));

        model->setRootItem(rootItem);
        // rootItem->removeRow(0);
    }

    connect(model, &AceTreeModel::stepChanged, this, [this](int step) {
        QTreeWidgetItem *root;
        if ((root = m_treeWidget->topLevelItem(0))) {
            m_treeWidget->setItemExpandedRecursively(root, true);
        }
    });

    auto editMenu = new QMenu("Edit(&E)");
    auto undoAction = editMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    auto redoAction = editMenu->addAction("Redo");
    redoAction->setShortcuts({QKeySequence::Redo, QKeySequence("Ctrl+Shift+Z")});
    menuBar()->addMenu(editMenu);

    connect(undoAction, &QAction::triggered, this, [this] {
        m_treeWidget->model()->previousStep(); //
    });
    connect(redoAction, &QAction::triggered, this, [this] {
        m_treeWidget->model()->nextStep(); //
    });

    m_treeWidget->setColumnWidth(0, 300);

    resize(800, 600);
}

MainWindow::~MainWindow() {
}