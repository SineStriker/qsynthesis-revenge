#ifndef QMWIDGETSHOST_P_H
#define QMWIDGETSHOST_P_H

#include "../QMWidgetsHost.h"
#include "private/QMCoreHost_p.h"

#include "../QMDecorateDir.h"

class QMWIDGETS_API QMWidgetsHostPrivate : public QMCoreHostPrivate {
    Q_DECLARE_PUBLIC(QMWidgetsHost)
public:
    QMWidgetsHostPrivate();
    ~QMWidgetsHostPrivate();

    void init();
    
    QList<QSharedPointer<QMDecorateDir>> decs;

    QMCoreFactory *createFactory() override;
};

#endif // QMWIDGETSHOST_P_H
