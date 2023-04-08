#include "WindowHandleFactory.h"

#include "IWindowHandle.h"
#include "Api/interfaces/IWindowHandlePlugin.h"

#include <private/qfactoryloader_p.h>

QSAPI_BEGIN_NAMESPACE

#if QT_CONFIG(settings)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
                          (QsApi_IWindowHandlePlugin_IID, QLatin1String("/windowhandles"), Qt::CaseInsensitive))
#endif

QStringList WindowHandleFactory::keys() {
#if QT_CONFIG(settings)
    return loader()->keyMap().values();
#else
    return QStringList();
#endif
}

QString WindowHandleFactory::requested() {
    QByteArray env = qgetenv("QSAPI_WINDOW_HANDLE");
    return env.isNull() ? "NativeWindow" : QString::fromLocal8Bit(env);
}

IWindowHandle *WindowHandleFactory::create(const QString &key, QMainWindow *parent) {
#if QT_CONFIG(settings)
    if (!key.isEmpty()) {
        IWindowHandle *inst = qLoadPlugin<IWindowHandle, IWindowHandlePlugin>(loader(), key, parent);
        if (inst)
            return inst;
        delete inst;
    }
#else
    Q_UNUSED(key);
#endif
    return nullptr;
}

IWindowHandle *WindowHandleFactory::create(QMainWindow *parent) {
    return create(requested(), parent);
}

QSAPI_END_NAMESPACE