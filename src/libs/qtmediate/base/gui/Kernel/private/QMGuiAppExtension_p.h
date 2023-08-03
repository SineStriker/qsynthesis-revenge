#ifndef QMGUIAPPEXTENSION_P_H
#define QMGUIAPPEXTENSION_P_H

#include <QAtomicInt>

#include <private/QMCoreAppExtension_p.h>

#include "../QMGuiAppExtension.h"

class QMGUI_EXPORT QMGuiInitFactory : public QMCoreInitFactory {
public:
    QMCoreConsole *createConsole(QObject *parent) override;
    QMCoreDecoratorV2 *createDecorator(QObject *parent) override;
};

class QMGUI_EXPORT QMGuiAppExtensionPrivate : public QMCoreAppExtensionPrivate {
    Q_DECLARE_PUBLIC(QMGuiAppExtension)
public:
    QMGuiAppExtensionPrivate();
    ~QMGuiAppExtensionPrivate();

    void init();

    QMCoreInitFactory *createFactory() override;

    static QAtomicInt globalImageCacheSerialNum;
};

#endif // QMGUIAPPEXTENSION_P_H
