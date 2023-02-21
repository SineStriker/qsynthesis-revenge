#include "QMWidgetsGlobal.h"

#include "private/QMetaTypeImpl_p.h"

void QMWidgets_Init() {
    Q_ENTER_ONCE

    QMCoreLib_Init();
    
    Register_QMetaTypeImpl();
}
