#ifndef LVAPPLICATION_H
#define LVAPPLICATION_H

#include "Kernel/LvElemApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<LvApplication *>(QCoreApplication::instance()))

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qApp, &LvApplication::stringUpdated, this, &T::reloadStrings);

class LvApplicationPrivate;

class LvApplication : public LvElemApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(LvApplication)
public:
    LvApplication(int &argc, char **argv);
    ~LvApplication();

protected:
    LvApplication(LvApplicationPrivate &d, int &argc, char **argv);
};

#endif // LVAPPLICATION_H
