#ifndef EVENTHELPER_H
#define EVENTHELPER_H

#include <QKeyEvent>

namespace Ev {

    bool keyIsDown(QEvent *event);

    bool isUnusableKey(Qt::Key key);

    bool isSpecialKey(Qt::Key key);

    bool isModifierKey(Qt::Key key);

    bool isSpecialShortcut(QKeySequence shortcut);

} // namespace Ev

#endif // EVENTHELPER_H
