#ifndef INAMEPLUGIN_H
#define INAMEPLUGIN_H

#include <QObject>
#include <QPluginLoader>

#include "QsGlobal.h"

QSAPI_BEGIN_NAMESPACE

class QSBASE_API INamePlugin : public QObject {
    Q_OBJECT
public:
    explicit INamePlugin(QObject *parent = nullptr);
    ~INamePlugin();

    /**
     * @brief path: Plugin library absolute path
     */
    QString path;

protected:
    static void setLastPluginPath(const QString &path);

public:
    template <class T>
    static T *load(QPluginLoader *ld);
};

template <class T>
T *INamePlugin::load(QPluginLoader *ld) {
    static_assert(std::is_base_of<INamePlugin, T>::value, "T should inherit from INamePlugin");
    if (!ld->load()) {
        return nullptr;
    }
    setLastPluginPath(ld->fileName());
    return qobject_cast<T *>(ld->instance());
}

QSAPI_END_NAMESPACE

#define QsApiPlugin QsApi::INamePlugin

#endif // INAMEPLUGIN_H
