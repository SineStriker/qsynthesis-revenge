#include "DsDistConfig.h"
#include "QsSystem.h"
#include "private/DsDistConfig_p.h"

#include <QDebug>

DsDistConfig::DsDistConfig() : DsDistConfig(*new DsDistConfigPrivate()) {
}

DsDistConfig::~DsDistConfig() {
}

QString DsDistConfig::appPluginDir() const {
    return appDir(AppPlugins);
}

DsDistConfig::DsDistConfig(DsDistConfigPrivate &d) : QsCoreConfig(d) {
    d.init();
}
