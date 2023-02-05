#ifndef DSDISTCONFIG_H
#define DSDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "Kernel/QsCoreConfig.h"

#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf DsDistConfig::instance()

class DsDistConfigPrivate;

class DsDistConfig : public QsCoreConfig {
    Q_DECLARE_PRIVATE(DsDistConfig)
public:
    DsDistConfig();
    ~DsDistConfig();

    enum ExtendedPlugins {
        DsEngine,
    };

protected:
    DsDistConfig(DsDistConfigPrivate &d);
};

#endif // DSDISTCONFIG_H
