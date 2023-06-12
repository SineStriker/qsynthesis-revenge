#ifndef QMAPPEXTENSION_P_H
#define QMAPPEXTENSION_P_H

#include "../QMAppExtension.h"
#include "private/QMGuiAppExtension_p.h"

class QMInitFactory : public QMGuiInitFactory {
public:
    QMCoreConsole *createConsole(QObject *parent) override;
    QMCoreDecoratorV2 *createDecorator(QObject *parent) override;
};

class QMAppExtensionPrivate : public QMGuiAppExtensionPrivate {
    Q_DECLARE_PUBLIC(QMAppExtension)
public:
    QMAppExtensionPrivate();
    ~QMAppExtensionPrivate();

    void init();

    QMCoreInitFactory *createFactory() override;
};

#endif // QMAPPEXTENSION_P_H
