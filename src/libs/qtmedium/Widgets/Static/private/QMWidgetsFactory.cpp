#include "QMWidgetsFactory.h"

#include "../QMConsole.h"
#include "../QMDecorator.h"

QMCoreDecorator *QMWidgetsFactory::createDecorator(QObject *parent) {
    return new QMDecorator(parent);
}

QMCoreConsole *QMWidgetsFactory::createConsole(QObject *parent) {
    return new QMConsole(parent);
}