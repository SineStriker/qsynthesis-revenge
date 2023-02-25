#ifndef QMDECORATEDIRPRIVATE_H
#define QMDECORATEDIRPRIVATE_H

#include "../QMDecorateDir.h"
#include "private/QMLocaleDir_p.h"

class QMWIDGETS_API QMDecorateDirPrivate : public QMLocaleDirPrivate {
    Q_DECLARE_PUBLIC(QMDecorateDir)
public:
    QMDecorateDirPrivate();
    ~QMDecorateDirPrivate();

    void init();

    bool loadNext(const QJsonObject &objDoc) override;
    void unloadNext() override;

    void unloadTheme();

    QList<RootItem> themes;
};

#endif // QMDECORATEDIRPRIVATE_H