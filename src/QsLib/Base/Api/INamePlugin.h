#ifndef INAMEPLUGIN_H
#define INAMEPLUGIN_H

#include <QObject>
#include <QPluginLoader>

#include "QsGlobal.h"

class QSBASE_API INamePlugin : public QObject {
    Q_OBJECT
public:
    explicit INamePlugin(QObject *parent = nullptr);
    ~INamePlugin();

    /**
     * @brief path: Plugin library absolute path
     */
    QString path;

    static void setLastPluginPath(const QString &path);

    template <class T>
    static T *load(QPluginLoader *ld) {
        static_assert(std::is_base_of<INamePlugin, T>::value, "T should inherit from INamePlugin");
        if (!ld->load()) {
            return nullptr;
        }
        setLastPluginPath(ld->fileName());
        return qobject_cast<T *>(ld->instance());
    }
};

#endif // INAMEPLUGIN_H
