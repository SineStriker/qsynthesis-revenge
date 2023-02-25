#ifndef QSABSTRACTMANAGER_H
#define QSABSTRACTMANAGER_H

#include <QObject>

#include "QsFrameworkGlobal.h"
#include "QMDisposable.h"

class QsAbstractManagerPrivate;

class QSFRAMEWORK_API QsAbstractManager : public QMDisposable {
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
