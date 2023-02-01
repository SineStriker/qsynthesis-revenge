#include "QUtaDistConfig.h"
#include "QsSystem.h"
#include "private/QUtaDistConfig_p.h"

#include <QDebug>

QUtaDistConfig::QUtaDistConfig() : QUtaDistConfig(*new QUtaDistConfigPrivate()) {
}

QUtaDistConfig::~QUtaDistConfig() {
}

QUtaDistConfig::QUtaDistConfig(QUtaDistConfigPrivate &d) : QsCoreConfig(d) {
     d.init();
}
