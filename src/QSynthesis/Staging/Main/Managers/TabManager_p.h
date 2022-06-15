#ifndef TABMANAGERPRIVATE_H
#define TABMANAGERPRIVATE_H

#include "../CentralManager_p.h"

#include "CentralTab.h"
#include "TabManager.h"

class MainWindow;

class TabManagerPrivate : public CentralManagerPrivate {
    Q_DECLARE_PUBLIC(TabManager)
public:
    TabManagerPrivate();
    ~TabManagerPrivate();

    void init();

    CentralTab *currentTab() const;
    CentralTab *tabAt(int index) const;
    int tabCount() const;

    bool tryCloseTab(int index);
};

#endif // TABMANAGERPRIVATE_H
