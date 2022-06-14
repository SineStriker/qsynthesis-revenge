#include "TabManager_p.h"

#include "DataManager.h"

#include "MainWindow.h"

TabManagerPrivate::TabManagerPrivate() {
}

TabManagerPrivate::~TabManagerPrivate() {
}

void TabManagerPrivate::init() {
    Q_Q(TabManager);

    w = qobject_cast<MainWindow *>(q->parent());
    Q_ASSERT(w);

    w->installEventFilter(q);

    // Connect Signals And Slots
    auto tabs = w->tabWidget();
    q->connect(tabs, &CentralTabWidget::currentTabTextChanged, q, &TabManager::_q_tabTitleChanged);
    q->connect(tabs, &CentralTabWidget::tabCloseRequested, q, &TabManager::_q_tabCloseRequested);
    q->connect(tabs, &CentralTabWidget::currentChanged, q, &TabManager::_q_tabIndexChanged);
    q->connect(tabs, &CentralTabWidget::tabBarClicked, q, &TabManager::_q_tabBarClicked);
}

CentralTab *TabManagerPrivate::currentTab() const {
    return qobject_cast<CentralTab *>(w->tabWidget()->currentWidget());
}

CentralTab *TabManagerPrivate::tabAt(int index) const {
    return qobject_cast<CentralTab *>(w->tabWidget()->widget(index));
}

int TabManagerPrivate::tabCount() const {
    return w->tabWidget()->count();
}

bool TabManagerPrivate::tryCloseTab(int index) {
    return false;
}
