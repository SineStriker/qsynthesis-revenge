#ifndef CHORUSKIT_QMSVG_P_H
#define CHORUSKIT_QMSVG_P_H

#include <QIcon>

namespace QMSvgPrivate {

    enum IconEngineHook {
        Create = 0x1000,
        Update,
        GetColor,
        SetColor,
    };

}

#endif // CHORUSKIT_QMSVG_P_H
