#ifndef LVDISTCONFIGPRIVATE_H
#define LVDISTCONFIGPRIVATE_H

#include "../LvDistConfig.h"
#include "Kernel/private/QsDistConfig_p.h"

#include <QHash>

class LvDistConfigPrivate : public QsDistConfigPrivate {
    Q_DECLARE_PUBLIC(LvDistConfig)
public:
    LvDistConfigPrivate();
    ~LvDistConfigPrivate();

    void init();

    void initByApp();
};

#endif // LVDISTCONFIGPRIVATE_H
