#ifndef QMWIDGETSFACTORY_H
#define QMWIDGETSFACTORY_H

#include "QMWidgetsGlobal.h"
#include "private/QMCoreFactory.h"

class QMWIDGETS_API QMWidgetsFactory : public QMCoreFactory {
public:
    QMCoreDecoratorV2 *createDecorator(QObject *parent = nullptr) override;

    QMCoreConsole *createConsole(QObject *parent = nullptr) override;
};

#endif // QMWIDGETSFACTORY_H
