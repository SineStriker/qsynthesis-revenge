#include "QMDecorateDir.h"
#include "private/QMDecorateDir_p.h"

#include "QMDecorator.h"

QMDecorateDir::QMDecorateDir() : QMDecorateDir(*new QMDecorateDirPrivate()) {
}

QMDecorateDir::QMDecorateDir(const QString &dir) : QMDecorateDir() {
    setDir(dir);
}

QMDecorateDir::~QMDecorateDir() {
}

QMDecorateDir::QMDecorateDir(QMDecorateDirPrivate &d) : QMLocaleDir(d) {
    d.init();
}
