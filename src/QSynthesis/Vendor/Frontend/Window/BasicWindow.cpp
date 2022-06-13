#include "BasicWindow.h"
#include "CMenuBar.h"

#include <FramelessWidgetsHelper>

FRAMELESSHELPER_USE_NAMESPACE

using namespace Global;

BasicWindow::BasicWindow(QWidget *parent) : FramelessMainWindow(parent) {
    auto helper = FramelessWidgetsHelper::get(this);

    // Insert Menu Bar To Title Bar
    auto bar = new CMenuBar();

    // New Window Bar
    m_titleBar = new CWindowBarV2(bar);

    // setMenuWidget(): make the menu widget become the first row of the window.
    this->setMenuWidget(m_titleBar);
    m_titleBar->setWidget(this);

    helper->setTitleBarWidget(m_titleBar);
    helper->setSystemButton(m_titleBar->minButton(), SystemButtonType::Minimize);
    helper->setSystemButton(m_titleBar->maxButton(), SystemButtonType::Maximize);
    helper->setSystemButton(m_titleBar->closeButton(), SystemButtonType::Close);
    helper->setHitTestVisible(bar); // IMPORTANT!
}

BasicWindow::~BasicWindow() {
}

void BasicWindow::setMenuBar(QMenuBar *menuBar) {
    auto helper = FramelessWidgetsHelper::get(this);
    auto orgBar = this->menuBar();
    if (orgBar) {
        helper->setHitTestVisible(orgBar, false);
    }
    m_titleBar->setMenuBar(menuBar);
    helper->setHitTestVisible(menuBar);
}

QMenuBar *BasicWindow::menuBar() const {
    return m_titleBar->menuBar();
}
