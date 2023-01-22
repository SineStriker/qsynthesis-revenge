#ifndef IWINDOWFACTORY_H
#define IWINDOWFACTORY_H

#include "IWindowHandle.h"

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API IWindowFactory : public QObject {
    Q_OBJECT
public:
    IWindowFactory(QObject *parent = nullptr);
    ~IWindowFactory();

    virtual IWindowHandle *create(QMainWindow *win);
};

#define IWindowFactory_IID "QSynthesis.Plugin.Framework.WindowFactory"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IWindowFactory, IWindowFactory_IID)
QT_END_NAMESPACE

#endif // IWINDOWFACTORY_H
