#ifndef QMWIDGETSHOST_H
#define QMWIDGETSHOST_H

#include "QMCoreHost.h"
#include "QMWidgetsGlobal.h"

#ifdef qmHost
#    undef qmHost
#endif
#define qmHost qobject_cast<QMWidgetsHost *>(QMCoreHost::instance())

class QMWidgetsHostPrivate;

class QMWIDGETS_API QMWidgetsHost : public QMCoreHost {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMWidgetsHost)
public:
    explicit QMWidgetsHost(QObject *parent = nullptr);
    ~QMWidgetsHost();

protected:
    QMWidgetsHost(QMWidgetsHostPrivate &d, QObject *parent = nullptr);
};

#endif // QMWIDGETSHOST_H
