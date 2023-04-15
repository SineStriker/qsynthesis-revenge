#include "QMWidgetsFactory.h"

#include "../QMConsole.h"
#include "../QMDecoratorV2.h"

QMCoreDecoratorV2 *QMWidgetsFactory::createDecorator(QObject *parent) {
    return new QMDecoratorV2(parent);
}

QMCoreConsole *QMWidgetsFactory::createConsole(QObject *parent) {
    return new QMConsole(parent);
}