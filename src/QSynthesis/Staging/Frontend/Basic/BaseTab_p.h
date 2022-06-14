#ifndef BASETABPRIVATE_H
#define BASETABPRIVATE_H

#include "BaseTab.h"

class BaseTabPrivate {
    Q_DECLARE_PUBLIC(BaseTab)
public:
    BaseTabPrivate();
    virtual ~BaseTabPrivate();

    void init();

    BaseTab *q_ptr;

    QIcon icon;
    QString tabName;
    bool active;
};
#endif // BASETABPRIVATE_H
