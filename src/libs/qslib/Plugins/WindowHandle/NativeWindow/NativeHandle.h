#ifndef NATIVEHANDLE_H
#define NATIVEHANDLE_H

#include "Api/IWindowHandle.h"

QSAPI_USING_NAMESPACE

class NativeHandlePrivate;

class Q_DECL_EXPORT NativeHandle : public IWindowHandle {
    Q_OBJECT
    Q_DECLARE_PRIVATE(NativeHandle)
public:
    explicit NativeHandle(QMainWindow *parent);
    ~NativeHandle();

public:
    void setup() override;

    void setMenuBar(QMenuBar *menuBar) override;
    QMenuBar *menuBar() const override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    NativeHandle(NativeHandlePrivate &d, QMainWindow *parent);
};

#endif // NATIVEHANDLE_H
