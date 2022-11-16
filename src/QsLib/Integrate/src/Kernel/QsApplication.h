#ifndef QSELEMAPPLICATION_H
#define QSELEMAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QsApplication *>(QCoreApplication::instance()))

class QsApplicationPrivate;

#include "qsintegrate_global.h"

class QSINTEGRATE_API QsApplication : public CApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsApplication)
public:
    QsApplication(int &argc, char **argv);
    ~QsApplication();

public:
    static QString mainTitle();
    static QString windowTitle();
    static QString errorTitle();
    static QString untitledFileName();
    static QString unsavedPrefix();
    static QString deletedPrefix();
    static QString QsFileManagerName();
    static QString allFilesFilter();

protected:
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv);

private:
    void q_screenRatioChanged(double dpi);
};

#endif // QSELEMAPPLICATION_H
