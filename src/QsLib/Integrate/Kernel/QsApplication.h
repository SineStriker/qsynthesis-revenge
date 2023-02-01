#ifndef QSAPPLICATION_H
#define QSAPPLICATION_H

#include <QApplication>

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QsApplication *>(QCoreApplication::instance()))

class QsApplicationPrivate;

#include "QsIntegrateGlobal.h"

class QSINTEGRATE_API QsApplication : public QApplication {
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

#endif // QSAPPLICATION_H
