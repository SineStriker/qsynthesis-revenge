#ifndef IWINDOWHANDLE_H
#define IWINDOWHANDLE_H

#include <QMainWindow>

#include "qsframework_global.h"

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

protected:
    IWindowHandle(IWindowHandlePrivate &d, QMainWindow *parent);

    QScopedPointer<IWindowHandlePrivate> d_ptr;
};

#endif // IWINDOWHANDLE_H
