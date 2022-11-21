#ifndef QUTAAPPLICATIONPRIVATE_H
#define QUTAAPPLICATIONPRIVATE_H

#include "../QUtaApplication.h"

#include "Kernel/private/QUtaElemApplication_p.h"

// #include "Managers/WindowManager.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

class QUtaApplicationPrivate : public QUtaElemApplicationPrivate {
    Q_DECLARE_PUBLIC(QUtaApplication)
public:
    QUtaApplicationPrivate();
    ~QUtaApplicationPrivate();

    void init();
    void deinit();

    // WindowManager *windowMgr;

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // QUTAAPPLICATIONPRIVATE_H
