#ifndef QMLOCALEDIR_H
#define QMLOCALEDIR_H

#include <QScopedPointer>

#include "QMSimpleVarExp.h"

class QMLocaleDirPrivate;

class QMCORELIB_API QMLocaleDir {
    Q_DECLARE_PRIVATE(QMLocaleDir)
public:
    QMLocaleDir();
    explicit QMLocaleDir(const QString &dir);
    virtual ~QMLocaleDir();

public:
    QMSimpleVarExp vars;
    bool autoRemove;

    QString dir() const;
    void setDir(const QString &dir);
    
    bool loadDefault(const QString &name);

    bool load(const QString &filename);
    void unload();

    static bool AutoDetectLocales;

protected:
    QMLocaleDir(QMLocaleDirPrivate &d);

    QScopedPointer<QMLocaleDirPrivate> d_ptr;
};

#endif // QMLOCALEDIR_H
