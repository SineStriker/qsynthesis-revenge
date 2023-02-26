#ifndef LVDISTCONFIG_H
#define LVDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "Kernel/QsCoreConfig.h"

#include "lvelem_global.h"

#ifdef qsConf
#undef qsConf
#endif
#define qsConf LvDistConfig::instance()

class LvDistConfigPrivate;

class LVELEM_API LvDistConfig : public QsCoreConfig {
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
