#include "NativeHandle_p.h"

#include "CMenuBar.h"

using namespace Global;

#define REPLACE_QMAINWINDOW_MENUBAR 0

#if REPLACE_QMAINWINDOW_MENUBAR
#    include <private/qlayout_p.h>

#    include <QLayout>
#    include <QMenuBar>

class HackLayout : public QLayout {
public:
    void replaceMenuBar(QMenuBar *menuBar) {
        auto d = static_cast<QLayoutPrivate *>(d_ptr.data());
        d->menubar = menuBar;
    }
};
#endif

NativeHandlePrivate::NativeHandlePrivate() {
}

NativeHandlePrivate::~NativeHandlePrivate() {
}

void NativeHandlePrivate::init() {
    titleBar = nullptr;
}

void NativeHandlePrivate::updateTitleBar() {
    titleBar->m_iconButton->setVisible(titleBarFlags & IWindowHandle::WindowIcon);
    titleBar->m_closeButton->setVisible(titleBarFlags & IWindowHandle::WindowCloseButton);
    titleBar->m_maxButton->setVisible(titleBarFlags & IWindowHandle::WindowMaximizeButton);
    titleBar->m_minButton->setVisible(titleBarFlags & IWindowHandle::WindowMinimizeButton);
    titleBar->setTitleVisible(titleBarFlags & IWindowHandle::WindowTitle);
}

void NativeHandlePrivate::setup_helper() {
    Q_Q(NativeHandle);

    // Not necessary, but better call this function, before the construction
    // of any Q(Core|Gui)Application instances.
    // FramelessHelper::Widgets::initialize();

    FramelessConfig::instance()->set(Option::WindowUseRoundCorners, true);
    FramelessWidgetsHelper::get(w)->extendsContentIntoTitleBar();
    m_helper.reset(new WidgetsSharedHelper(q));
    m_helper->setup(w);

    installTitleBar();
}

void NativeHandlePrivate::setMenuBar_helper(QMenuBar *menuBar) {
    auto helper = FramelessWidgetsHelper::get(w);
    auto orgBar = q_ptr->menuBar();
    if (orgBar) {
        helper->setHitTestVisible(orgBar, false);
    }
    helper->setHitTestVisible(menuBar);
    titleBar->setMenuBar(menuBar);

#if REPLACE_QMAINWINDOW_MENUBAR
    reinterpret_cast<HackLayout *>(w->layout())->replaceMenuBar(menuBar);
#endif
}

void NativeHandlePrivate::installTitleBar() {
    auto helper = FramelessWidgetsHelper::get(w);

    // Insert Menu Bar To Title Bar
    auto bar = new CMenuBar();

    // New Window Bar
    titleBar = new CWindowBarV2(bar);
    titleBar->installEventFilter(w);

    // setMenuWidget(): make the menu widget become the first row of the window.
    w->setMenuWidget(titleBar);
    titleBar->setWidget(w);

    helper->setTitleBarWidget(titleBar);
    helper->setSystemButton(titleBar->minButton(), SystemButtonType::Minimize);
    helper->setSystemButton(titleBar->maxButton(), SystemButtonType::Maximize);
    helper->setSystemButton(titleBar->closeButton(), SystemButtonType::Close);
    helper->setHitTestVisible(bar); // IMPORTANT!

#if REPLACE_QMAINWINDOW_MENUBAR
    reinterpret_cast<HackLayout *>(w->layout())->replaceMenuBar(bar);
#endif
}
