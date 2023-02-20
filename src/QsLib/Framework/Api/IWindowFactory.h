#ifndef IWINDOWFACTORY_H
#define IWINDOWFACTORY_H

#include "INamePlugin.h"
#include "IWindowHandle.h"

QSAPI_BEGIN_NAMESPACE

class QSFRAMEWORK_API IWindowFactory : public INamePlugin {
    Q_OBJECT
public:
    explicit IWindowFactory(QObject *parent = nullptr);
    ~IWindowFactory();

    /**
     * @brief Create a new window handle instance
     *
     * @param win Window pointer
     */
    virtual IWindowHandle *create(QMainWindow *win);
};

QSAPI_END_NAMESPACE

#define QsApi_IWindowFactory_IID "QSynthesis.Plugin.Framework.WindowFactory"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QsApi::IWindowFactory, QsApi_IWindowFactory_IID)
QT_END_NAMESPACE

#endif // IWINDOWFACTORY_H
