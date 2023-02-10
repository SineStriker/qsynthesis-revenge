#ifndef QSABSTRACTMANAGER_H
#define QSABSTRACTMANAGER_H

#include <QObject>

#include "QsGlobal.h"

class QsAbstractManagerPrivate;

class QSBASE_API QsAbstractManager : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsAbstractManager)
public:
    QsAbstractManager(QObject *parent = nullptr);
    ~QsAbstractManager();

    virtual bool load();
    virtual bool save();

protected:
    QsAbstractManager(QsAbstractManagerPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QsAbstractManagerPrivate> d_ptr;
};

#endif // QSABSTRACTMANAGER_H
