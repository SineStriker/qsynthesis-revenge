#ifndef QMCOREFACTORY_H
#define QMCOREFACTORY_H

#include <QString>

#include "../QsCoreDecorator.h"
#include "../QsCoreConsole.h"

class QMCORELIB_API QMCoreFactory {
public:
    virtual ~QMCoreFactory() = default;

public:
    virtual QsCoreDecorator *createDecorator(QObject *parent = nullptr);
    
    virtual QsCoreConsole *createConsole(QObject *parent = nullptr);
};

#endif // QMCOREFACTORY_H
