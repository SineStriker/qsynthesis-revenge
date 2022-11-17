#ifndef LVAPPLICATION_H
#define LVAPPLICATION_H

#include "Kernel/LvElemApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<LvApplication *>(QCoreApplication::instance()))

class LvApplicationPrivate;

class LvApplication : public LvElemApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(LvApplication)
public:
    LvApplication(int &argc, char **argv);
    ~LvApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

protected:
    LvApplication(LvApplicationPrivate &d, int &argc, char **argv);
};

#endif // LVAPPLICATION_H
