#ifndef LVELEMAPPLICATION_H
#define LVELEMAPPLICATION_H

#include "Kernel/QsApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<LvElemApplication *>(QCoreApplication::instance()))

class LvElemApplicationPrivate;

#include "lvelem_global.h"

class LVELEM_API LvElemApplication : public QsApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(LvElemApplication)
public:
    LvElemApplication(int &argc, char **argv);
    ~LvElemApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

protected:
    LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv);
};

#endif // LVELEMAPPLICATION_H
