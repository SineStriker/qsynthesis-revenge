#ifndef CDECORATEDIRPRIVATE_H
#define CDECORATEDIRPRIVATE_H

#include "../CDecorateDir.h"
#include "private/QsLocaleDir_p.h"

class CDecorateDirPrivate : public QsLocaleDirPrivate {
    Q_DECLARE_PUBLIC(CDecorateDir)
public:
    CDecorateDirPrivate();
    ~CDecorateDirPrivate();

    void init();

    void unloadTheme();
    
    QString themeKey;
};

#endif // CDECORATEDIRPRIVATE_H