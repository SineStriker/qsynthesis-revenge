#ifndef QSPLUGINDIR_H
#define QSPLUGINDIR_H

#include <QObject>
#include <QScopedPointer>
#include <QString>

#include "INamePlugin.h"

class QsPluginDirPrivate;

class QSBASE_API QsPluginDir {
    Q_DECLARE_PRIVATE(QsPluginDir)
public:
    QsPluginDir();
    virtual ~QsPluginDir();

public:
    bool autoRemove;

    void load(const QString &dir);
    void unload();

    bool hasPlugin() const;

    QObjectList instances() const;

    template <class T>
    QList<T *> plugins() const;

protected:
    QsPluginDir(QsPluginDirPrivate &d);

    QScopedPointer<QsPluginDirPrivate> d_ptr;
};

template <class T>
QList<T *> QsPluginDir::plugins() const {
    static_assert(std::is_base_of<INamePlugin, T>::value, "T should inherit from INamePlugin");
    QList<T *> res;
    auto insts = instances();
    for (const auto &item : qAsConst(insts)) {
        T *obj;
        if ((obj = qobject_cast<T *>(item))) {
            res.append(obj);
        }
    }
    return res;
}


#endif // QSPLUGINDIR_H
