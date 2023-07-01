#include "ShortcutContext_p.h"

#include <QActionEvent>

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::ShortcutContext::" << (func) << "():").space()

    ShortcutContext::ShortcutContext(QObject *parent) : QObject(parent) {
        lastFlushedAction = nullptr;
    }

    ShortcutContext::~ShortcutContext() {
        qDeleteAll(actionDataMap);
    }

    void ShortcutContext::addContext(QWidget *w) {
        if (!w) {
            myWarning(__func__) << "trying to add null widget";
            return;
        }

        if (shortcutContextWidgets.contains(w)) {
            myWarning(__func__) << "trying to add duplicated widget:" << w;
            return;
        }

        shortcutContextWidgets.insert(w);
        connect(w, &QObject::destroyed, this, &ShortcutContext::_q_shortcutContextDestroyed);

        shortcutContextAdded(w);

        w->installEventFilter(this);
    }

    void ShortcutContext::removeContext(QWidget *w) {
        auto it = shortcutContextWidgets.find(w);
        if (it == shortcutContextWidgets.end()) {
            myWarning(__func__) << "widget does not exist:" << w;
            return;
        }

        w->removeEventFilter(this);

        shortcutContextRemoved(w);

        disconnect(w, &QObject::destroyed, this, &ShortcutContext::_q_shortcutContextDestroyed);
        shortcutContextWidgets.erase(it);
    }

    bool ShortcutContext::hasContext(QWidget *w) const {
        return shortcutContextWidgets.contains(w);
    }

    bool ShortcutContext::hasShortcut(const QKeySequence &key) const {
        return shortcutMap.contains(key);
    }

    bool ShortcutContext::eventFilter(QObject *obj, QEvent *event) {
        switch (event->type()) {
            case QEvent::ActionAdded: {
                auto e = static_cast<QActionEvent *>(event);
                auto w = qobject_cast<QWidget *>(obj);
                addAction(e->action(), w);
                flushAction(e->action());
                break;
            }
            case QEvent::ActionChanged: {
                auto e = static_cast<QActionEvent *>(event);
                flushAction(e->action());
                break;
            }
            case QEvent::ActionRemoved: {
                auto e = static_cast<QActionEvent *>(event);
                auto w = qobject_cast<QWidget *>(obj);
                removeAction(e->action(), w);
                break;
            }
            default:
                break;
        }

        return QObject::eventFilter(obj, event);
    }

    void ShortcutContext::shortcutContextAdded(QWidget *w) {
        const auto &actions = w->actions();
        for (const auto &action : actions) {
            addAction(action, w);
        }
        for (const auto &action : actions) {
            flushAction(action);
        }
    }

    void ShortcutContext::shortcutContextRemoved(QWidget *w) {
        // We cannot query widgets' actions because the widget maybe destroyed
        QList<QAction *> actions;
        for (const auto &item : qAsConst(actionDataMap)) {
            if (item->widgets.contains(w)) {
                actions.append(item->action);
            }
        }

        for (const auto &action : qAsConst(actions)) {
            removeAction(action, w);
        }
    }

    void ShortcutContext::addAction(QAction *action, QWidget *w) {
        auto it = actionDataMap.find(action);
        if (it != actionDataMap.end()) {
            it.value()->widgets.insert(w);
            return;
        }

        auto data = new ActionData();
        data->action = action;
        data->widgets.insert(w);
        actionDataMap.insert(action, data);
    }

    void ShortcutContext::removeAction(QAction *action, QWidget *w) {
        auto it = actionDataMap.find(action);
        if (it == actionDataMap.end()) {
            return;
        }

        auto &set = it.value()->widgets;
        set.remove(w);
        if (!set.isEmpty()) {
            return;
        }

        for (const auto &sh : action->shortcuts()) {
            shortcutMap.remove(sh);
        }

        delete it.value();
        actionDataMap.erase(it);
    }

    void ShortcutContext::flushAction(QAction *action) {
        if (lastFlushedAction == action) {
            return;
        }

        auto data = actionDataMap.value(action);
        if (!data) {
            return;
        }

        if (!lastFlushedAction) {
            // Reset this pointer after all actions has been handled
            QTimer::singleShot(0, this, [this]() {
                lastFlushedAction = nullptr; //
            });
        }
        lastFlushedAction = action;

        // Remove old keys
        for (const auto &key : qAsConst(data->keys)) {
            shortcutMap.remove(key);
        }

        QList<QKeySequence> keys;
        QMChronSet<QKeySequence> duplicatedKeys;
        for (const auto &sh : action->shortcuts()) {
            if (sh.isEmpty())
                continue;
            if (shortcutMap.contains(sh)) {
                duplicatedKeys.append(sh);
                continue;
            }
            keys.append(sh);
            shortcutMap.insert(sh, data);
        }

        // Avoid recursive signal handling
        action->blockSignals(true);
        action->setShortcuts(keys);
        action->blockSignals(false);

        data->keys = keys;

        if (!duplicatedKeys.isEmpty()) {
            myWarning(__func__) << "duplicated shortcuts detected" << action << duplicatedKeys.values();
        }
    }

    void ShortcutContext::_q_shortcutContextDestroyed(QObject *obj) {
        auto w = static_cast<QWidget *>(obj);
        shortcutContextRemoved(w);
        shortcutContextWidgets.remove(w);
    }
}