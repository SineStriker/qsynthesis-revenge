#ifndef TABMANAGERPRIVATE_H
#define TABMANAGERPRIVATE_H

#include "BaseManager_p.h"

#include "CentralTab.h"
#include "TabManager.h"

class MainWindow;

class TabManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(TabManager)
public:
    TabManagerPrivate();
    ~TabManagerPrivate();

    void init();

    CentralTab *currentTab() const;
    CentralTab *tabAt(int index) const;
    int tabCount() const;

    bool tryCloseTab(int index);

    MainWindow *w;
};

#endif // TABMANAGERPRIVATE_H
