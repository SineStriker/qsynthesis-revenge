#include "TestWindow.h"

#include "CCoupleTabFrame.h"
#include "CMenu.h"
#include "Central/CentralTabWidget.h"

#include "DataManager.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QScreen>

TestWindow::TestWindow(QWidget *parent) : BasicWindow(parent) {
    // Menu Bar
    auto bar = menuBar();
    auto menu1 = new CMenu("Menu 1");
    auto menu2 = new CMenu("Menu 2");
    menu1->addActions({new QAction("Action 1"), new QAction("Action 2")});
    menu2->addActions({new QAction("Action 3"), new QAction("Action 4")});
    bar->addMenu(menu1);
    bar->addMenu(menu2);

    // Widget
    auto frame = new CCoupleTabFrame();

    auto CARD = [](const QString &text, QWidget *w) {
        auto card = new CCoupleTabBarCard(text);
        card->setWidget(w);
        return card;
    };

    frame->leftBar()->firstBar()->addCard(CARD("card1", new QLabel("card1")));
    frame->leftBar()->firstBar()->addCard(CARD("card2", new QLabel("card2")));
    frame->leftBar()->secondBar()->addCard(CARD("card3", new QLabel("card3")));

    frame->rightBar()->firstBar()->addCard(CARD("card4", new QLabel("card4")));
    frame->rightBar()->firstBar()->addCard(CARD("card5", new QLabel("card5")));
    frame->rightBar()->secondBar()->addCard(CARD("card6", new QLabel("card6")));

    frame->topBar()->firstBar()->addCard(CARD("card7", new QLabel("card7")));
    frame->topBar()->firstBar()->addCard(CARD("card8", new QLabel("card8")));
    frame->topBar()->secondBar()->addCard(CARD("card9", new QLabel("card9")));

    frame->bottomBar()->firstBar()->addCard(CARD("card10", new QLabel("card10")));
    frame->bottomBar()->firstBar()->addCard(CARD("card11", new QLabel("card11")));
    frame->bottomBar()->secondBar()->addCard(CARD("card12", new QLabel("card12")));

    frame->setWidget(new QPushButton("123"));

    // Tabs
    auto tabs = new CentralTabWidget();
    setCentralWidget(tabs);
    tabs->addTab(frame, "tab-1");
    tabs->addTab(new QLabel("2"), "tab-2");
    tabs->addTab(new QLabel("3"), "tab-3");
    tabs->addTab(new QLabel("11"), "tab-111111");
    tabs->addTab(new QLabel("22"), "tab-222222");
    tabs->addTab(new QLabel("33"), "tab-33333333");
    tabs->addTab(new QLabel("44"), "tab-4444");
    tabs->addTab(new QLabel("55"), "tab-555555");

    setWindowTitle(qApp->applicationDisplayName());
    resize(1280, 720);

    Q_TR_NOTIFY(TestWindow)
}

TestWindow::~TestWindow() {
}

void TestWindow::reloadStrings() {
    m_titleBar->reloadStrings();
}
