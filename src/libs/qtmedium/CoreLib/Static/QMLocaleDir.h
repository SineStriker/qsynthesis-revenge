#ifndef QMLOCALEDIR_H
#define QMLOCALEDIR_H

#include <QScopedPointer>

#include "Text/QMSimpleVarExp.h"

class QMLocaleDirPrivate;

class QMCORELIB_API QMLocaleDir {
    Q_DECLARE_PRIVATE(QMLocaleDir)
public:
    QMLocaleDir();
    virtual ~QMLocaleDir();

public:
    QMSimpleVarExp vars;
    bool autoRemove;

    bool load(const QString &filename);
    void unload();

    static bool AutoDetectLocales;

protected:
    QMLocaleDir(QMLocaleDirPrivate &d);

    QScopedPointer<QMLocaleDirPrivate> d_ptr;
};

#endif // QMLOCALEDIR_H
