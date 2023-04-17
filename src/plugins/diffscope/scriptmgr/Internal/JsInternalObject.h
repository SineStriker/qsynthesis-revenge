//
// Created by Crs_1 on 2023/4/16.
//

#ifndef CHORUSKIT_JSINTERNALOBJECT_H
#define CHORUSKIT_JSINTERNALOBJECT_H

#include <QFormLayout>
#include <QJSEngine>

#include "AddOn/ScriptMgrAddOn.h"

namespace ScriptMgr::Internal {

    class JsInternalObject : public QObject {
        Q_OBJECT
    public:
        explicit JsInternalObject(QJSEngine *engine);
        void setAddOn(ScriptMgrAddOn *addOn);

    public slots:
        QString jsTr(const QString &text);
        QString getLang();
        void infoMsgBox(const QString &title, const QString &message);
        bool questionMsgBox(const QString &title, const QString &message, const QString &defaultButton);
        QJSValue form(const QString &title, const QVariantList &widgets);
        void registerScript(const QString &id, int flags);
        void registerScriptSet(const QString &id, const QStringList &childrenId, int flags);

    protected:
        QJSEngine *engine;
        ScriptMgrAddOn *addOn = nullptr;
        bool _createFormWidget(QFormLayout &formLayout, const QVariantMap& widgetParams, QJSValue &ret, int index);
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECT_H
