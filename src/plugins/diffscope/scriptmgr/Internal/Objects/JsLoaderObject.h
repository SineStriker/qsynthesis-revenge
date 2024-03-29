#ifndef CHORUSKIT_JSLOADEROBJECT_H
#define CHORUSKIT_JSLOADEROBJECT_H

#include <QJSValue>
#include <QMap>
#include <QObject>

namespace ScriptMgr::Internal {

    class ScriptLoader;

    class JsLoaderObject : public QObject {
        Q_OBJECT
    public slots:
        void registerScript(const QString &id, const QStringList &role, const QString &shortcut) const;
        void registerScriptSet(const QString &id, const QStringList &childrenId, const QStringList &role, const QStringList &childrenShortcut) const;
        QString uuid() const;
        void assertFalse() const;
        QString version() const;
        QString getLang() const;
        QString coreMsgBox(const QString &title, const QString &message, const QString &icon, const QStringList &buttons, const QString &defaultButton) const;
        bool versionMismatchWarning(const QString &id, const QString &requiredVersion) const;

        QStringList handleExtList() const;
        QJSValue getHandleExt(const QString &name, const QJSValue &internalObjectJSValue) const;
    private:
        friend class ScriptLoader;
        JsLoaderObject(ScriptLoader *loader);
        ScriptLoader *m_loader;
    };

} // Internal

#endif // CHORUSKIT_JSLOADEROBJECT_H
