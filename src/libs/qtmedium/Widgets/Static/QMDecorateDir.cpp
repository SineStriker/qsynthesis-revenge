#include "QMDecorateDir.h"
#include "private/QMDecorateDir_p.h"

QMDecorateDir::QMDecorateDir() : QMDecorateDir(*new QMDecorateDirPrivate()) {
}

QMDecorateDir::~QMDecorateDir() {
}

QMDecorateDir::QMDecorateDir(QMDecorateDirPrivate &d) : QMLocaleDir(d) {
    d.init();
}
