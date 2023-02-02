#ifndef QSLOCALEDIR_H
#define QSLOCALEDIR_H

#include <QScopedPointer>

#include "QSimpleVarExp.h"

class QsLocaleDirPrivate;

class QSBASE_API QsLocaleDir {
    Q_DECLARE_PRIVATE(QsLocaleDir)
public:
    QsLocaleDir();
    explicit QsLocaleDir(const QString &dir);
    virtual ~QsLocaleDir();

public:
    QSimpleVarExp vars;
    bool autoRemove;

    void setDir(const QString &dir);
    bool loadDefault(const QString &binName);

    virtual bool load(const QString &filename);
    virtual void unload();

protected:
    QsLocaleDir(QsLocaleDirPrivate &d);

    QScopedPointer<QsLocaleDirPrivate> d_ptr;
};

#endif // QSLOCALEDIR_H