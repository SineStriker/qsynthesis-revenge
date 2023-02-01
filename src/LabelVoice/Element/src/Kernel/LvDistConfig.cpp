#include "LvDistConfig.h"
#include "QsSystem.h"
#include "private/LvDistConfig_p.h"

#include <QDebug>

LvDistConfig::LvDistConfig() : LvDistConfig(*new LvDistConfigPrivate()) {
}

LvDistConfig::~LvDistConfig() {
}

LvDistConfig::LvDistConfig(LvDistConfigPrivate &d) : QsCoreConfig(d) {
     d.init();
}
