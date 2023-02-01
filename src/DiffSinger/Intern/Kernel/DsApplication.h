#ifndef DSAPPLICATION_H
#define DSAPPLICATION_H

#include "QsApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<DsApplication *>(QCoreApplication::instance()))

class DsApplicationPrivate;

class DsApplication : public QsApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsApplication)
public:
    DsApplication(int &argc, char **argv);
    ~DsApplication();

protected:
    DsApplication(DsApplicationPrivate &d, int &argc, char **argv);
};

#endif // DSAPPLICATION_H
