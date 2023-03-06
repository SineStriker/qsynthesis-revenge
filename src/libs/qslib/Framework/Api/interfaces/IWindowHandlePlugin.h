#ifndef IWINDOWHANDLEPLUGIN_H
#define IWINDOWHANDLEPLUGIN_H

#include <QObject>

#include "QsFrameworkGlobal.h"

class QMainWindow;

QSAPI_BEGIN_NAMESPACE

class IWindowHandle;

class QSFRAMEWORK_API IWindowHandlePlugin : public QObject {
    Q_OBJECT
public:
    explicit IWindowHandlePlugin(QObject *parent = nullptr);
    ~IWindowHandlePlugin();

    virtual IWindowHandle *create(const QString &key, QMainWindow *parent) = 0;
};

QSAPI_END_NAMESPACE

#define QsApi_IWindowHandlePlugin_IID "org.ChorusKit.QsLib.IWindowHandlePlugin"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QsApi::IWindowHandlePlugin, QsApi_IWindowHandlePlugin_IID)
QT_END_NAMESPACE


#endif // IWINDOWHANDLEPLUGIN_H
