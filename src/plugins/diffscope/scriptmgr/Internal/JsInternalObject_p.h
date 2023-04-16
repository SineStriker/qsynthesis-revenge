//
// Created by Crs_1 on 2023/4/16.
//

#ifndef CHORUSKIT_JSINTERNALOBJECTPRIVATE_H
#define CHORUSKIT_JSINTERNALOBJECTPRIVATE_H

namespace ScriptMgr::Internal {

    class JsInternalObjectPrivate {
        Q_DECLARE_PUBLIC(JsInternalObject);

    public:
        explicit JsInternalObjectPrivate(QJSEngine *engine, ScriptMgrAddOn *addOn);

        JsInternalObject *q_ptr;
        QJSEngine *engine;
        ScriptMgrAddOn *addOn;
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECTPRIVATE_H
