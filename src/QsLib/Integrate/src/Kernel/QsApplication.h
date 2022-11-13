#ifndef QSELEMAPPLICATION_H
#define QSELEMAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QsApplication *>(QCoreApplication::instance()))

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qApp, &QsApplication::stringUpdated, this, &T::reloadStrings);

class QsApplicationPrivate;

#include "qsintegrate_global.h"

class QSINTEGRATE_API QsApplication : public CApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsApplication)
public:
    QsApplication(int &argc, char **argv);
    ~QsApplication();

public:
    virtual void reloadStrings();
    virtual void reloadScreen();

    static QString mainTitle();
    static QString windowTitle();
    static QString errorTitle();
    static QString untitledFileName();
    static QString unsavedPrefix();
    static QString deletedPrefix();
    static QString fileManagerName();
    static QString allFilesFilter();

protected:
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv);

private:
    void q_screenRatioChanged(qreal dpi);

signals:
    void stringUpdated();
};

#endif // QSELEMAPPLICATION_H
