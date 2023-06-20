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

}

#endif // QMNAMESPACE_H
