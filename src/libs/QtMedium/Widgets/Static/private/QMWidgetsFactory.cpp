#include "QMWidgetsFactory.h"

#include "../CConsole.h"
#include "../CDecorator.h"

QsCoreDecorator *QMWidgetsFactory::createDecorator(QObject *parent) {
    return new CDecorator(parent);
}

QsCoreConsole *QMWidgetsFactory::createConsole(QObject *parent) {
    return new CConsole(parent);
}