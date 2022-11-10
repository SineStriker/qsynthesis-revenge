#ifndef LVELEMAPPLICATION_H
#define LVELEMAPPLICATION_H

#include "CApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<LvElemApplication *>(QCoreApplication::instance()))

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qApp, &LvElemApplication::stringUpdated, this, &T::reloadStrings);

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

    static QString mainTitle();
    static QString windowTitle();
    static QString errorTitle();
    static QString untitledFileName();
    static QString unsavedPrefix();
    static QString deletedPrefix();
    static QString fileManagerName();
    static QString allFilesFilter();

protected:
    LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv);

private:
    void q_screenRatioChanged(qreal dpi);

signals:
    void stringUpdated();
};

#endif // LVELEMAPPLICATION_H
