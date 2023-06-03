#ifndef CHORUSKIT_SHORTCUTCONTEXT_P_H
#define CHORUSKIT_SHORTCUTCONTEXT_P_H

#include <QObject>

#include "IWindow_p.h"

namespace Core {

    class ShortcutContext : public QObject {
        Q_OBJECT
    public:
        explicit ShortcutContext(QObject *parent = nullptr);
        ~ShortcutContext();

        void addContext(QWidget *w);
        void removeContext(QWidget *w);

        bool hasContext(QWidget *w) const;
        bool hasShortcut(const QKeySequence &key) const;

        QList<QWidget *> contexts() const {
            return shortcutContextWidgets.values();
        }

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        struct ActionData {
            QAction *action;
            QSet<QWidget *> widgets;
            QList<QKeySequence> keys;
        };
        QHash<QKeySequence, ActionData *> shortcutMap;
        QHash<QAction *, ActionData *> actionDataMap;
        QSet<QWidget *> shortcutContextWidgets;
        QAction *lastFlushedAction;

        void shortcutContextAdded(QWidget *w);
        void shortcutContextRemoved(QWidget *w);

        void addAction(QAction *action, QWidget *w);
        void removeAction(QAction *action, QWidget *w);
        void flushAction(QAction *action);

        void _q_shortcutContextDestroyed(QObject *obj);
    };

}



#endif // CHORUSKIT_SHORTCUTCONTEXT_P_H
