#include "QMGlobal.h"

#include <QMetaType>

#include "private/QMNamespace_p.h"

void QMCoreLib_Init() {
    Q_ENTER_ONCE
    Register_QMNamespace();
}
