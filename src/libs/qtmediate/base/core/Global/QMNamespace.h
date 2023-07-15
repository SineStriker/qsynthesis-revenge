#ifndef QMNAMESPACE_H
#define QMNAMESPACE_H

#include <QObject>

#include "QMGlobal.h"

namespace QM {

    Q_NAMESPACE_EXPORT(QMCORE_EXPORT)

    enum Direction {
        Forward,
        Backward,
    };
    Q_ENUM_NS(Direction)

    enum Priority {
        Primary = 16,
        Secondary = 32,
    };
    Q_ENUM_NS(Priority)

    enum ClickState {
        CS_Normal,
        CS_Hover,
        CS_Pressed,
        CS_Disabled,
        CS_Normal_Checked,
        CS_Hover_Checked,
        CS_Pressed_Checked,
        CS_Disabled_Checked,
    };
    Q_ENUM_NS(ClickState)

}

#endif // QMNAMESPACE_H
