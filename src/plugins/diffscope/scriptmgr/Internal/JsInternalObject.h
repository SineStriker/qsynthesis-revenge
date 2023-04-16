//
// Created by Crs_1 on 2023/4/16.
//

#ifndef CHORUSKIT_JSINTERNALOBJECT_H
#define CHORUSKIT_JSINTERNALOBJECT_H

#include <QJSEngine>

#include "AddOn/ScriptMgrAddOn.h"

namespace ScriptMgr::Internal {

    class JsInternalObjectPrivate;

    class JsInternalObject : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(JsInternalObject);
    public:
        explicit JsInternalObject(QJSEngine *engine, ScriptMgrAddOn *addOn);

    public slots:
        QString jsTr(const QString &text);
        QString getLang();
        QJSValue form(const QJSValue &val);
        void registerScript(const QString &id, int flags);
        void registerScriptSet(const QString &id, const QStringList &childrenId, int flags);

    protected:
        JsInternalObject(JsInternalObjectPrivate *d);
        QScopedPointer<JsInternalObjectPrivate> d_ptr;
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECT_H
