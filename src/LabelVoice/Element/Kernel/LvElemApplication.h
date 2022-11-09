#ifndef LVELEMAPPLICATION_H
#define LVELEMAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<LvElemApplication *>(QCoreApplication::instance()))

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qApp, &LvApplication::stringUpdated, this, &T::reloadStrings);

class LvElemApplicationPrivate;

#include "lvelem_global.h"

class LVELEM_API LvElemApplication : public CApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(LvElemApplication)
public:
    LvElemApplication(int &argc, char **argv);
    ~LvElemApplication();

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
    QString appExtDir() const;
    QString desktopDir();

protected:
    LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv);

private:
    void q_screenRatioChanged(qreal dpi);

signals:
    void stringUpdated();
};

#endif // LVELEMAPPLICATION_H
