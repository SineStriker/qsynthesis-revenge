#ifndef DSDISTCONFIG_P_H
#define DSDISTCONFIG_P_H

#include "../DsDistConfig.h"
#include "Kernel/private/QsCoreConfig_p.h"

#include <QHash>

class DsDistConfigPrivate : public QsCoreConfigPrivate {
    Q_DECLARE_PUBLIC(DsDistConfig)
public:
    DsDistConfigPrivate();
    ~DsDistConfigPrivate();

    void init();

    void initByApp();
};

#endif // DSDISTCONFIG_P_H
