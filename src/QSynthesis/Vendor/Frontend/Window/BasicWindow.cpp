#include "BasicWindow.h"
#include "CMenuBar.h"

#include <FramelessWidgetsHelper>

FRAMELESSHELPER_USE_NAMESPACE

using namespace Global;

BasicWindow::BasicWindow(QWidget *parent) : FramelessMainWindow(parent) {
    // Insert Menu Bar To Title Bar
    m_titleBar = new CWindowBarV2(new CMenuBar());

    // setMenuWidget(): make the menu widget become the first row of the window.
    this->setMenuWidget(m_titleBar);
    m_titleBar->setWidget(this);

    FramelessWidgetsHelper *helper = FramelessWidgetsHelper::get(this);
    helper->setTitleBarWidget(m_titleBar);
    helper->setSystemButton(m_titleBar->minButton(), SystemButtonType::Minimize);
    helper->setSystemButton(m_titleBar->maxButton(), SystemButtonType::Maximize);
    helper->setSystemButton(m_titleBar->closeButton(), SystemButtonType::Close);
    helper->setHitTestVisible(menuBar()); // IMPORTANT!
}

BasicWindow::~BasicWindow() {
}

void BasicWindow::setMenuBar(QMenuBar *menuBar) {
    m_titleBar->setMenuBar(menuBar);
}

QMenuBar *BasicWindow::menuBar() const {
    return m_titleBar->menuBar();
}
