#ifndef __DSELEMAPPLICATION_H__
#define __DSELEMAPPLICATION_H__

#include "Kernel/QsApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<DsElemApplication *>(QCoreApplication::instance()))

class DsElemApplicationPrivate;

#include "dscore_global.h"

class DSCORE_API DsElemApplication : public QsApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsElemApplication)
public:
    DsElemApplication(int &argc, char **argv);
    ~DsElemApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

protected:
    DsElemApplication(DsElemApplicationPrivate &d, int &argc, char **argv);
};

#endif // __DSELEMAPPLICATION_H__
