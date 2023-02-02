#include "CDecorateDir_p.h"

#include "../CDecorator.h"

CDecorateDirPrivate::CDecorateDirPrivate() {
}

CDecorateDirPrivate::~CDecorateDirPrivate() {
    Q_Q(QsLocaleDir);
    if (q->autoRemove)
        unloadTheme();
}

void CDecorateDirPrivate::init() {
}

void CDecorateDirPrivate::unloadTheme() {
    if (!themeKey.isEmpty()) {
        qIDec->removeThemeConfig(themeKey);
        themeKey.clear();
    }
}