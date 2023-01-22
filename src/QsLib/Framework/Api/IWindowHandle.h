#ifndef IWINDOWHANDLE_H
#define IWINDOWHANDLE_H

#include <QMainWindow>

#include "QsFrameworkGlobal.h"

class IWindowHandlePrivate;

class QSFRAMEWORK_API IWindowHandle : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(IWindowHandle)
public:
    IWindowHandle(QMainWindow *parent);
    ~IWindowHandle();

public:
    QMainWindow *window() const;
    void setParent(QObject *) = delete;

    virtual void setup();

    virtual void setMenuBar(QMenuBar *menuBar);
    virtual QMenuBar *menuBar() const;

    enum TitleBarFlag {
        WindowIcon = 0x1,
        WindowTitle = 0x2,
        WindowMinimizeButton = 0x4,
        WindowMaximizeButton = 0x8,
        WindowCloseButton = 0x10,
        TitleBarFlagMask = WindowIcon | WindowTitle |                    //
                           WindowMinimizeButton | WindowMaximizeButton | //
                           WindowCloseButton,
    };
    Q_DECLARE_FLAGS(TitleBarFlags, TitleBarFlag);

    void setTitleBarFlags(TitleBarFlags flags);
    TitleBarFlags titleBarFlags();

protected:
    IWindowHandle(IWindowHandlePrivate &d, QMainWindow *parent);

    QScopedPointer<IWindowHandlePrivate> d_ptr;
};

#endif // IWINDOWHANDLE_H
