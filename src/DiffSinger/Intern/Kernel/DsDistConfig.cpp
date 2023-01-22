#include "DsDistConfig.h"
#include "QsSystem.h"
#include "private/DsDistConfig_p.h"

#include <QDebug>

DsDistConfig::DsDistConfig() : DsDistConfig(*new DsDistConfigPrivate()) {
}

DsDistConfig::~DsDistConfig() {
}

DsDistConfig::DsDistConfig(DsDistConfigPrivate &d) : QsDistConfig(d) {
     d.init();
}
