//
// Created by Crs_1 on 2023/4/16.
//

#ifndef CHORUSKIT_JSINTERNALOBJECTPRIVATE_H
#define CHORUSKIT_JSINTERNALOBJECTPRIVATE_H

namespace BatProc::Internal {

    class JsInternalObjectPrivate {
        Q_DECLARE_PUBLIC(JsInternalObject);
    public:
        explicit JsInternalObjectPrivate(QJSEngine *engine, BatProcAddOn *addOn);
        JsInternalObject *q_ptr;
        QJSEngine *engine;
        BatProcAddOn *addOn;
    };

} // Internal

#endif // CHORUSKIT_JSINTERNALOBJECTPRIVATE_H
