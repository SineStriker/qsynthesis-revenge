#ifndef DSDISTCONFIG_H
#define DSDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "DsCoreGlobal.h"
#include "Kernel/QsCoreConfig.h"


#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf static_cast<DsDistConfig *>(QsCoreConfig::instance())

class DsDistConfigPrivate;

class DSCORE_API DsDistConfig : public QsCoreConfig {
    Q_DECLARE_PRIVATE(DsDistConfig)
public:
    DsDistConfig();
    ~DsDistConfig();

    enum AppDirType {
        AppPlugins = DirType::UserDir + 1,
    };

    enum ExtendedPlugins {
        DsEngine,
    };

    QString appPluginDir() const;

protected:
    DsDistConfig(DsDistConfigPrivate &d);
};

#endif // DSDISTCONFIG_H
