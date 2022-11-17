#ifndef NATIVEWINDOWFACTORY_H
#define NATIVEWINDOWFACTORY_H

#include "Api/IWindowFactory.h"

class NativeWindowFactoryPrivate;

class Q_DECL_EXPORT NativeWindowFactory : public IWindowFactory {
    Q_OBJECT
    Q_INTERFACES(IWindowFactory)
    Q_PLUGIN_METADATA(IID IWindowFactory_IID FILE "plugin.json")
    Q_DECLARE_PRIVATE(NativeWindowFactory)
public:
    explicit NativeWindowFactory(QObject *parent = nullptr);
    ~NativeWindowFactory();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

public:
    IWindowHandle *create(QMainWindow *win) override;

protected:
    QScopedPointer<NativeWindowFactoryPrivate> d_ptr;

signals:
};

#endif // NATIVEWINDOWFACTORY_H
