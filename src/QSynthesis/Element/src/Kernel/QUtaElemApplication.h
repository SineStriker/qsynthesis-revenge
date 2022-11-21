#ifndef __QUTAELEMAPPLICATION_H__
#define __QUTAELEMAPPLICATION_H__

#include "Kernel/QsApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QUtaElemApplication *>(QCoreApplication::instance()))

class QUtaElemApplicationPrivate;

#include "qutaelem_global.h"

class QUTAELEM_API QUtaElemApplication : public QsApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QUtaElemApplication)
public:
    QUtaElemApplication(int &argc, char **argv);
    ~QUtaElemApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

protected:
    QUtaElemApplication(QUtaElemApplicationPrivate &d, int &argc, char **argv);
};

#endif // __QUTAELEMAPPLICATION_H__