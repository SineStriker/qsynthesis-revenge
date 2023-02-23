#ifndef QMCOREHOST_P_H
#define QMCOREHOST_P_H

#include "../QMCoreHost.h"

#include "QMCoreFactory.h"

class QMCORELIB_API QMCoreHostPrivate {
    Q_DECLARE_PUBLIC(QMCoreHost)
public:
    QMCoreHostPrivate();
    virtual ~QMCoreHostPrivate();

    void init();

    QMCoreHost *q_ptr;
    
    bool isAboutToQuit;

    virtual QMCoreFactory *createFactory();

private:
    QScopedPointer<QMCoreFactory> fac;

    QsCoreDecorator *s_dec;
    QsCoreConsole *s_cs;
};

#endif // QMCOREHOST_P_H
