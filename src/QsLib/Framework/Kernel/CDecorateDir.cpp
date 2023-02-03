#include "CDecorateDir.h"
#include "private/CDecorateDir_p.h"

#include "CDecorator.h"

CDecorateDir::CDecorateDir() : CDecorateDir(*new CDecorateDirPrivate()) {
}

CDecorateDir::CDecorateDir(const QString &dir) : CDecorateDir() {
    setDir(dir);
}

CDecorateDir::~CDecorateDir() {
}

CDecorateDir::CDecorateDir(CDecorateDirPrivate &d) : QsLocaleDir(d) {
    d.init();
}
