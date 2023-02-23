#ifndef CDECORATEDIRPRIVATE_H
#define CDECORATEDIRPRIVATE_H

#include "../CDecorateDir.h"
#include "private/QsLocaleDir_p.h"

class QMWIDGETS_API CDecorateDirPrivate : public QsLocaleDirPrivate {
    Q_DECLARE_PUBLIC(CDecorateDir)
public:
    CDecorateDirPrivate();
    ~CDecorateDirPrivate();

    void init();

    bool loadNext(const QJsonObject &objDoc) override;
    void unloadNext() override;

    void unloadTheme();

    QList<RootItem> themes;
};

#endif // CDECORATEDIRPRIVATE_H