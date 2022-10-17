#include "EventHelper.h"

#include <QApplication>
#include <QMimeData>

bool Ev::keyIsDown(QEvent *event) {
    QList<QEvent::Type> types{QEvent::KeyPress, QEvent::ShortcutOverride};
    return types.contains(event->type());
}

bool Ev::isUnusableKey(Qt::Key key) {
    QList<Qt::Key> keys{Qt::Key_CapsLock, Qt::Key_NumLock, Qt::Key_ScrollLock, Qt::Key_Meta};
    return keys.contains(key);
}

bool Ev::isSpecialKey(Qt::Key key) {
    QList<Qt::Key> keys{Qt::Key_Tab, Qt::Key_Escape, Qt::Key_Return, Qt::Key_Enter};
    return keys.contains(key);
}

bool Ev::isModifierKey(Qt::Key key) {
    QList<Qt::Key> keys{Qt::Key_Control, Qt::Key_Shift, Qt::Key_Alt};
    return keys.contains(key);
}

bool Ev::isSpecialShortcut(QKeySequence shortcut) {
    QList<QKeySequence> shortcuts{QKeySequence("Tab")};
    return shortcuts.contains(shortcut);
}
