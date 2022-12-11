#ifndef LVDISTCONFIG_H
#define LVDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "Kernel/QsDistConfig.h"

#include "lvelem_global.h"

#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf LvDistConfig::instance()

class LvDistConfigPrivate;

class LVELEM_API LvDistConfig : public QsDistConfig {
    Q_DECLARE_PRIVATE(LvDistConfig)
public:
    LvDistConfig();
    ~LvDistConfig();

    enum ExtendedPlugins {
        LvProjTemplate,
    };

protected:
    LvDistConfig(LvDistConfigPrivate &d);
};

#endif // LVDISTCONFIG_H
