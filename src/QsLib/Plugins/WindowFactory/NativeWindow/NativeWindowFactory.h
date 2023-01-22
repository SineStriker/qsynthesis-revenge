#ifndef NATIVEWINDOWFACTORY_H
#define NATIVEWINDOWFACTORY_H

#include "Api/IWindowFactory.h"

class NativeWindowFactoryPrivate;

class Q_DECL_EXPORT NativeWindowFactory : public IWindowFactory {
    Q_OBJECT
    Q_INTERFACES(IWindowFactory)
    Q_PLUGIN_METADATA(IID IWindowFactory_IID FILE "plugin.json")
public:
    explicit NativeWindowFactory(QObject *parent = nullptr);
    ~NativeWindowFactory();

public:
    IWindowHandle *create(QMainWindow *win) override;

protected:
    QScopedPointer<NativeWindowFactoryPrivate> d_ptr;
};

#endif // NATIVEWINDOWFACTORY_H
