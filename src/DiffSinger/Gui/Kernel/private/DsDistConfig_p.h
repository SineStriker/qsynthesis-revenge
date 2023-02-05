#ifndef __DSDISTCONFIG_P_H__
#define __DSDISTCONFIG_P_H__

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

#endif // __DSDISTCONFIG_P_H__