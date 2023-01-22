#ifndef QSELEMAPPLICATION_H
#define QSELEMAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QsApplication *>(QCoreApplication::instance()))

class QsApplicationPrivate;

#include "QsIntegrateGlobal.h"

class QSINTEGRATE_API QsApplication : public CApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsApplication)
public:
    QsApplication(int &argc, char **argv);
    ~QsApplication();

public:
    static QString untitledFileName();
    static QString unsavedPrefix();
    static QString deletedPrefix();

protected:
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv);

    QScopedPointer<QsApplicationPrivate> d_ptr;
};

#endif // QSELEMAPPLICATION_H
