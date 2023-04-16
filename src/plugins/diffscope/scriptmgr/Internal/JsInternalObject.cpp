//
// Created by Crs_1 on 2023/4/16.
//

#include "JsInternalObject.h"
#include "JsInternalObject_p.h"
#include "QMCoreDecoratorV2.h"

namespace ScriptMgr::Internal {

    JsInternalObject::JsInternalObject(QJSEngine *engine, ScriptMgrAddOn *addOn)
        : JsInternalObject(new JsInternalObjectPrivate(engine, addOn)) {
    }

    QString JsInternalObject::jsTr(const QString &text) {
        return tr(text.toLocal8Bit());
    }

    QString JsInternalObject::getLang() {
        return qIDec->locale();
    }

    QJSValue JsInternalObject::form(const QJSValue &val) {
        Q_D(JsInternalObject);
        return d->engine->newObject();
    }

    void JsInternalObject::registerScript(const QString &id, int flags) {
    }

    void JsInternalObject::registerScriptSet(const QString &id, const QStringList &childrenId, int flags) {
    }

    JsInternalObject::JsInternalObject(JsInternalObjectPrivate *d) : d_ptr(d) {
        d->q_ptr = this;
    }

    JsInternalObjectPrivate::JsInternalObjectPrivate(QJSEngine *engine, ScriptMgrAddOn *addOn)
        : engine(engine), addOn(addOn) {
    }

}