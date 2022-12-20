#ifndef DSAPPLICATION_H
#define DSAPPLICATION_H

#include "Kernel/DsElemApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<DsApplication *>(QCoreApplication::instance()))

class DsApplicationPrivate;

class DsApplication : public DsElemApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsApplication)
public:
    DsApplication(int &argc, char **argv);
    ~DsApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

protected:
    void receiveMessage(quint32 instanceId, const QByteArray &message) override;

protected:
    DsApplication(DsApplicationPrivate &d, int &argc, char **argv);
};

#endif // DSAPPLICATION_H
