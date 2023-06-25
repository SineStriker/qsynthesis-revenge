#ifndef CHORUSKIT_JSINTERNALOBJECT_H
#define CHORUSKIT_JSINTERNALOBJECT_H

#include <QFormLayout>
#include <QJSEngine>

namespace ScriptMgr::Internal {

    class ScriptMgrAddOn;

    class JsInternalObject : public QObject {
        Q_OBJECT
    public:
        explicit JsInternalObject(ScriptMgrAddOn *addOn);
        ~JsInternalObject();

    public slots:
        QString getLang() const;
        QString msgBox(const QString &title, const QString &message, const QString &icon, const QStringList &buttons,
                   const QString &defaultButton);
        void infoMsgBox(const QString &title, const QString &message) const;
        bool questionMsgBox(const QString &title, const QString &message, const QString &defaultButton) const;
        QJSValue form(const QString &title, const QVariantList &widgets, QJSValue listener) const;
        void assertFalse();

    protected:
        ScriptMgrAddOn *addOn = nullptr;
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECT_H
