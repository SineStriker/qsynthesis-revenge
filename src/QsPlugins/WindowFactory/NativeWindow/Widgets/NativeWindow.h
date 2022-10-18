#ifndef NATIVEWINDOW_H
#define NATIVEWINDOW_H

#include <FramelessMainWindow>

#include "CWindowBarV2.h"

class NativeWindowPrivate;

class Q_DECL_EXPORT NativeWindow : public FRAMELESSHELPER_NAMESPACE::FramelessMainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(NativeWindow)
public:
    explicit NativeWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});
    ~NativeWindow();

    using Super = FRAMELESSHELPER_NAMESPACE::FramelessMainWindow;

public:
    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;

protected:
    NativeWindow(NativeWindowPrivate &d, QWidget *parent = nullptr, Qt::WindowFlags flags = {});

    QScopedPointer<NativeWindowPrivate> d_ptr;

signals:
};

#endif // NATIVEWINDOW_H
