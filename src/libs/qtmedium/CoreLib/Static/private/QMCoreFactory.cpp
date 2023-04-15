#include "QMCoreFactory.h"

QMCoreDecoratorV2 *QMCoreFactory::createDecorator(QObject *parent) {
    return new QMCoreDecoratorV2(parent);
}

QMCoreConsole *QMCoreFactory::createConsole(QObject *parent) {
    return new QMCoreConsole(parent);
}
