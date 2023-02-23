#ifndef QMWIDGETSFACTORY_H
#define QMWIDGETSFACTORY_H

#include "QMWidgetsGlobal.h"
#include "private/QMCoreFactory.h"

class QMWIDGETS_API QMWidgetsFactory : public QMCoreFactory {
public:
    QsCoreDecorator *createDecorator(QObject *parent = nullptr) override;

    QsCoreConsole *createConsole(QObject *parent = nullptr) override;
};

#endif // QMWIDGETSFACTORY_H
