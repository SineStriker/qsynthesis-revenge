#ifndef QMCOREFACTORY_H
#define QMCOREFACTORY_H

#include <QString>

#include "../QMCoreDecoratorV2.h"
#include "../QMCoreConsole.h"

class QMCORELIB_API QMCoreFactory {
public:
    virtual ~QMCoreFactory() = default;

public:
    virtual QMCoreDecoratorV2 *createDecorator(QObject *parent = nullptr);
    
    virtual QMCoreConsole *createConsole(QObject *parent = nullptr);
};

#endif // QMCOREFACTORY_H
