#ifndef LVAPPLICATION_H
#define LVAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<LvApplication *>(QCoreApplication::instance()))

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qApp, &LvApplication::stringUpdated, this, &T::reloadStrings);

class LvApplicationPrivate;

class LvApplication : public CApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(LvApplication)
public:
    LvApplication(int &argc, char **argv);
    ~LvApplication();

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
    QString allFilesFilter() const;

    QString appDataPath() const;
    QString appTempPath() const;
    QString appPluginDir() const;
    QString desktopDir();

protected:
    LvApplication(LvApplicationPrivate &d, int &argc, char **argv);

private:
    void q_screenRatioChanged(qreal dpi);

signals:
    void stringUpdated();
};

#endif // LVAPPLICATION_H
