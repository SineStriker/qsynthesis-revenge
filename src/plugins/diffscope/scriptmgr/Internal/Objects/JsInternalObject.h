#ifndef CHORUSKIT_JSINTERNALOBJECT_H
#define CHORUSKIT_JSINTERNALOBJECT_H

#include <QFormLayout>
#include <QJSEngine>

namespace ScriptMgr::Internal {

    class ScriptMgrAddOn;
    class JsLoaderObject;

    class JsInternalObject : public QObject {
        Q_OBJECT
    public:
        explicit JsInternalObject(ScriptMgrAddOn *addOn);
        ~JsInternalObject();

    public slots:
        QString msgBox(const QString &title, const QString &message, const QString &icon, const QStringList &buttons, const QString &defaultButton) const;
        QJSValue form(const QString &title, const QVariantList &widgets, QJSValue listener) const;
        bool startTransaction(const QString &id, const QJSValue &index) const;
        void endTransaction(const QString &id, const QJSValue &index) const;
    protected:
        friend class JsLoaderObject;
        ScriptMgrAddOn *addOn;
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECT_H
