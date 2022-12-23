#ifndef DSDISTCONFIG_H
#define DSDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "Kernel/QsDistConfig.h"

#include "dscore_global.h"

#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf DsDistConfig::instance()

class DsDistConfigPrivate;

class DSCORE_API DsDistConfig : public QsDistConfig {
    Q_DECLARE_PRIVATE(DsDistConfig)
public:
    DsDistConfig();
    ~DsDistConfig();

    enum ExtendedPlugins {
        DiffHost,
    };

protected:
    DsDistConfig(DsDistConfigPrivate &d);
};

#endif // DSDISTCONFIG_H
