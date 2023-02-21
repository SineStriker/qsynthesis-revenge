#ifndef QMNAMEPLUGIN_H
#define QMNAMEPLUGIN_H

#include <QObject>
#include <QPluginLoader>

#include "QMGlobal.h"

class QMCORELIB_API QMNamePlugin : public QObject {
    Q_OBJECT
public:
    explicit QMNamePlugin(QObject *parent = nullptr);
    ~QMNamePlugin();

    /**
     * @brief path: Plugin library absolute path
     */
    QString path;

protected:
    static void setLastPluginPath(const QString &path);

public:
    template <class T>
    static T *load(QPluginLoader *ld);
    static QMNamePlugin *load(QPluginLoader *ld);
};

template <class T>
T *QMNamePlugin::load(QPluginLoader *ld) {
    static_assert(std::is_base_of<QMNamePlugin, T>::value, "T should inherit from QMNamePlugin");
    if (!ld->load()) {
        return nullptr;
    }
    setLastPluginPath(ld->fileName());
    return qobject_cast<T *>(ld->instance());
}

inline QMNamePlugin *QMNamePlugin::load(QPluginLoader *ld) {
    return QMNamePlugin::load<QMNamePlugin>(ld);
}

#endif // QMNAMEPLUGIN_H
