#ifndef EVENTHELPER_H
#define EVENTHELPER_H

#include <QKeyEvent>

#include "qsframework_global.h"

namespace Ev {

    QSFRAMEWORK_API bool keyIsDown(QEvent *event);

    QSFRAMEWORK_API bool isUnusableKey(Qt::Key key);

    QSFRAMEWORK_API bool isSpecialKey(Qt::Key key);

    QSFRAMEWORK_API bool isModifierKey(Qt::Key key);

    QSFRAMEWORK_API bool isSpecialShortcut(QKeySequence shortcut);

} // namespace Ev

#endif // EVENTHELPER_H
