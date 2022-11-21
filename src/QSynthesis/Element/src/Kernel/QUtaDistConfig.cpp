#include "QUtaDistConfig.h"
#include "SystemHelper.h"
#include "private/QUtaDistConfig_p.h"

#include <QDebug>

QUtaDistConfig::QUtaDistConfig() : QUtaDistConfig(*new QUtaDistConfigPrivate()) {
}

QUtaDistConfig::~QUtaDistConfig() {
}

QUtaDistConfig::QUtaDistConfig(QUtaDistConfigPrivate &d) : QsDistConfig(d) {
     d.init();
}
