#include "DsDistConfig.h"
#include "SystemHelper.h"
#include "private/DsDistConfig_p.h"

#include <QDebug>

DsDistConfig::DsDistConfig() : DsDistConfig(*new DsDistConfigPrivate()) {
}

DsDistConfig::~DsDistConfig() {
}

DsDistConfig::DsDistConfig(DsDistConfigPrivate &d) : QsDistConfig(d) {
     d.init();
}
