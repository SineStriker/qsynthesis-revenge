#include "QMCoreFactory.h"

QsCoreDecorator *QMCoreFactory::createDecorator(QObject *parent) {
    return new QsCoreDecorator(parent);
}

QsCoreConsole *QMCoreFactory::createConsole(QObject *parent) {
    return new QsCoreConsole(parent);
}
