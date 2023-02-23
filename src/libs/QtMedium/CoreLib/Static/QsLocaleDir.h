#ifndef QSLOCALEDIR_H
#define QSLOCALEDIR_H

#include <QScopedPointer>

#include "QMSimpleVarExp.h"

class QsLocaleDirPrivate;

class QMCORELIB_API QsLocaleDir {
    Q_DECLARE_PRIVATE(QsLocaleDir)
public:
    QsLocaleDir();
    explicit QsLocaleDir(const QString &dir);
    virtual ~QsLocaleDir();

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
    QsLocaleDir(QsLocaleDirPrivate &d);

    QScopedPointer<QsLocaleDirPrivate> d_ptr;
};

#endif // QSLOCALEDIR_H
