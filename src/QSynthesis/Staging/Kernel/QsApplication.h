#ifndef QSAPPLICATION_H
#define QSAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QsApplication *>(QCoreApplication::instance()))

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qApp, &QsApplication::stringUpdated, this, &T::reloadStrings);

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

public:
    void reloadStrings();
    void reloadScreen();

    QString mainTitle() const;
    QString windowTitle() const;
    QString errorTitle() const;
    QString untitledFileName() const;
    QString unsavedPrefix() const;
    QString deletedPrefix() const;
    QString fileManagerName() const;

    QString appDataPath() const;
    QString appTempPath() const;

protected:
#ifdef Q_QDOC
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv);
#else
    QsApplication(QsApplicationPrivate &d, int &argc, char **argv,
                  int _internal = ApplicationFlags);
#endif

private:
    void q_screenRatioChanged(qreal dpi);

signals:
    void stringUpdated();
};

#endif // QSAPPLICATION_H
