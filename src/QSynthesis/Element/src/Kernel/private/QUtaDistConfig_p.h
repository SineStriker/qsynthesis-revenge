#ifndef __QUTADISTCONFIG_P_H__
#define __QUTADISTCONFIG_P_H__

#include "../QUtaDistConfig.h"
#include "Kernel/private/QsDistConfig_p.h"

#include <QHash>

class QUtaDistConfigPrivate : public QsDistConfigPrivate {
    Q_DECLARE_PUBLIC(QUtaDistConfig)
public:
    QUtaDistConfigPrivate();
    ~QUtaDistConfigPrivate();

    void init();

    void initByApp();
};

#endif // __QUTADISTCONFIG_P_H__