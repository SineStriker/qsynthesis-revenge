#include "NativeWindow_p.h"

#include "CMenuBar.h"

#include <FramelessWidgetsHelper>

FRAMELESSHELPER_USE_NAMESPACE

using namespace Global;

NativeWindowPrivate::NativeWindowPrivate() {
}

void NativeWindowPrivate::init() {
    Q_Q(NativeWindow);
    auto helper = FramelessWidgetsHelper::get(q);

    // Insert Menu Bar To Title Bar
    auto bar = new CMenuBar();

    // New Window Bar
    titleBar = new CWindowBarV2(bar);
    titleBar->installEventFilter(q);

    // setMenuWidget(): make the menu widget become the first row of the window.
    q->setMenuWidget(titleBar);
    titleBar->setWidget(q);

    helper->setTitleBarWidget(titleBar);
    helper->setSystemButton(titleBar->minButton(), SystemButtonType::Minimize);
    helper->setSystemButton(titleBar->maxButton(), SystemButtonType::Maximize);
    helper->setSystemButton(titleBar->closeButton(), SystemButtonType::Close);
    helper->setHitTestVisible(bar); // IMPORTANT!
}
