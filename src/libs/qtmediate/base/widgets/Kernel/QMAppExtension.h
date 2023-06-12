#ifndef QMAPPEXTENSION_H
#define QMAPPEXTENSION_H

#include <QMGuiAppExtension.h>

#include "QMWidgetsGlobal.h"

#ifdef qAppExt
#    undef qAppExt
#endif
#define qAppExt qobject_cast<QMAppExtension *>(QMGuiAppExtension::instance())

class QMAppExtensionPrivate;

class QMWIDGETS_EXPORT QMAppExtension : public QMGuiAppExtension {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMAppExtension)
public:
    explicit QMAppExtension(QObject *parent = nullptr);
    ~QMAppExtension();

protected:
    QMAppExtension(QMAppExtensionPrivate &d, QObject *parent = nullptr);
};

#endif // QMAPPEXTENSION_H
