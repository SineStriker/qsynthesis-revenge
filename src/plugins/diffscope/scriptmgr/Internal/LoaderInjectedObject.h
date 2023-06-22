#ifndef CHORUSKIT_LOADERINJECTEDOBJECT_H
#define CHORUSKIT_LOADERINJECTEDOBJECT_H

#include <QObject>

namespace ScriptMgr::Internal {

    class ScriptLoader;

    class LoaderInjectedObject: public QObject {
        Q_OBJECT
    public slots:
        void registerScript(const QString &id, const QStringList &role, const QString &shortcut) const;
        void registerScriptSet(const QString &id, const QStringList &childrenId, const QStringList &role, const QStringList &childrenShortcut) const;
        QString uuid() const;
    private:
        friend class ScriptLoader;
        LoaderInjectedObject(ScriptLoader *loader);
        ScriptLoader *m_loader;
    };

} // Internal

#endif // CHORUSKIT_LOADERINJECTEDOBJECT_H
