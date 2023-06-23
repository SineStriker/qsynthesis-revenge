#ifndef CHORUSKIT_JSINTERNALOBJECT_H
#define CHORUSKIT_JSINTERNALOBJECT_H

#include <QFormLayout>
#include <QJSEngine>

#define JS_PROP_ASSERT(obj, prop, type) (obj.contains(#prop) && obj.value(#prop).canConvert(QVariant::type))
#define JS_PROP_AS(obj, prop, type) (obj.value(#prop).to##type())

namespace ScriptMgr::Internal {

    class ScriptMgrAddOn;

    class JsInternalObject : public QObject {
        Q_OBJECT
    public:
        explicit JsInternalObject(ScriptMgrAddOn *addOn);
        ~JsInternalObject();

    public slots:
        QString getLang() const;
        void infoMsgBox(const QString &title, const QString &message) const;
        bool questionMsgBox(const QString &title, const QString &message, const QString &defaultButton) const;
        QJSValue form(const QString &title, const QVariantList &widgets, QJSValue listener) const;

    protected:
        ScriptMgrAddOn *addOn = nullptr;
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECT_H
