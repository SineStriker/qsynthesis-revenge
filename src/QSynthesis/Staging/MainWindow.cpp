#include "MainWindow.h"
#include "CMenu.h"

#include "DataManager.h"

#include "ActionManager.h"
#include "EventManager.h"
#include "TabManager.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QScreen>

MainWindow::MainWindow(EditType editType, QWidget *parent) : BasicWindow(parent) {
    m_editType = editType;

    setAcceptDrops(true);

    m_layout = new QVBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    m_widget = new QWidget();
    m_widget->setAttribute(Qt::WA_StyledBackground);
    m_widget->setLayout(m_layout);

    m_tabs = new CentralTabWidget();
    m_frame = new CCoupleTabFrame();

    if (m_editType == Files) {
        m_layout->addWidget(m_tabs->moveOutTabBarWidget());
    }
    m_layout->addWidget(m_frame);

    m_frame->setWidget(m_tabs);
    this->setCentralWidget(m_widget);

    m_tabMgr = new TabManager(this);
    m_eventMgr = new EventManager(this);
    m_actionMgr = new ActionManager(this);

    m_tabMgr->load();
    m_eventMgr->load();
    m_actionMgr->load();

    Q_TR_NOTIFY(MainWindow)
}

MainWindow::~MainWindow() {
}

void MainWindow::reloadStrings() {
    m_titleBar->reloadStrings();
    m_actionMgr->reloadStrings();
}

CentralTabWidget *MainWindow::tabWidget() const {
    return m_tabs;
}

CCoupleTabFrame *MainWindow::frame() const {
    return m_frame;
}

MainWindow::EditType MainWindow::editType() const {
    return m_editType;
}

TabManager *MainWindow::tabMgr() const {
    return m_tabMgr;
}

EventManager *MainWindow::eventMgr() const {
    return m_eventMgr;
}

ActionManager *MainWindow::actionMgr() const {
    return m_actionMgr;
}
