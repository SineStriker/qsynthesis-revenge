#ifndef CENTRALTAB_P_H
#define CENTRALTAB_P_H

#include "BaseTab_p.h"
#include "CentralTab.h"

class CentralTabPrivate : public BaseTabPrivate {
    Q_DECLARE_PUBLIC(CentralTab)
public:
    CentralTabPrivate();
    virtual ~CentralTabPrivate();

    void init();

    bool edited;
    QString filename;

    virtual QString setTabNameProxy(const QString &tabName); // Set the title of tab
    virtual QString tabText();
};

#endif // CENTRALTAB_P_H
