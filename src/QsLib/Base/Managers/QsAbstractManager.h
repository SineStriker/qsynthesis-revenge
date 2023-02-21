#ifndef QSABSTRACTMANAGER_H
#define QSABSTRACTMANAGER_H

#include <QObject>

#include "QsGlobal.h"
#include "QMDisposable.h"

class QsAbstractManagerPrivate;

class QSBASE_API QsAbstractManager : public QMDisposable {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsAbstractManager)
public:
    explicit QsAbstractManager(QObject *parent = nullptr);
    ~QsAbstractManager();

protected:
    QsAbstractManager(QsAbstractManagerPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QsAbstractManagerPrivate> d_ptr;
};

#endif // QSABSTRACTMANAGER_H
