#ifndef WINDOWHANDLEFACTORY_H
#define WINDOWHANDLEFACTORY_H

#include <QMainWindow>
#include <QStringList>

#include "QsFrameworkGlobal.h"

QSAPI_BEGIN_NAMESPACE

class IWindowHandle;

class QSFRAMEWORK_API WindowHandleFactory {
public:
    static QStringList keys();
    static QString requested();
    static IWindowHandle *create(const QString &key, QMainWindow *parent);
    static IWindowHandle *create(QMainWindow *parent);
};

QSAPI_END_NAMESPACE

#endif // WINDOWHANDLEFACTORY_H
