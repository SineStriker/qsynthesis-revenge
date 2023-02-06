#ifndef DSDISTCONFIG_H
#define DSDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "Kernel/QsCoreConfig.h"
#include "DsGuiGlobal.h"

#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf static_cast<DsDistConfig *>(QsCoreConfig::instance())

class DsDistConfigPrivate;

class DSGUI_API DsDistConfig : public QsCoreConfig {
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
