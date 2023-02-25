#ifndef QMCOREFACTORY_H
#define QMCOREFACTORY_H

#include <QString>

#include "../QMCoreDecorator.h"
#include "../QMCoreConsole.h"

class QMCORELIB_API QMCoreFactory {
public:
    virtual ~QMCoreFactory() = default;

public:
    virtual QMCoreDecorator *createDecorator(QObject *parent = nullptr);
    
    virtual QMCoreConsole *createConsole(QObject *parent = nullptr);
};

#endif // QMCOREFACTORY_H
