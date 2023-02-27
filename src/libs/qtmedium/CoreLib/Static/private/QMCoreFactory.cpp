#include "QMCoreFactory.h"

QMCoreDecorator *QMCoreFactory::createDecorator(QObject *parent) {
    return new QMCoreDecorator(parent);
}

QMCoreConsole *QMCoreFactory::createConsole(QObject *parent) {
    return new QMCoreConsole(parent);
}
