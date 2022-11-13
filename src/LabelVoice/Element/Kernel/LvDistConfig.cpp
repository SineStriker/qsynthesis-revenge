#include "LvDistConfig.h"
#include "SystemHelper.h"
#include "private/LvDistConfig_p.h"

#include <QDebug>

LvDistConfig::LvDistConfig() : LvDistConfig(*new LvDistConfigPrivate()) {
}

LvDistConfig::~LvDistConfig() {
}

LvDistConfig::LvDistConfig(LvDistConfigPrivate &d) : QsDistConfig(d) {
     d.init();
}
