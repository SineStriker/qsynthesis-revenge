#ifndef QSAPPLICATION_H
#define QSAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QsApplication *>(QCoreApplication::instance()))

class QsApplicationPrivate;

class QsApplication : public CApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsApplication)
public:
#ifdef Q_QDOC
    QsApplication(int &argc, char **argv);
#else
    QsApplication(int &argc, char **argv, int _internal = ApplicationFlags);
#endif
    ~QsApplication();

protected:
#ifdef Q_QDOC
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv);
#else
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv, int _internal = ApplicationFlags);
#endif

private:
    void q_screenRatioChanged(qreal dpi);
};

#endif // QSAPPLICATION_H
