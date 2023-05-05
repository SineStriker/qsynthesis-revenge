#ifndef IWINDOW_P_H
#define IWINDOW_P_H

#include <QHash>
#include <QSet>
#include <QTimer>

#include <Collections/QMChronMap.h>
#include <Collections/QMChronSet.h>

#include "IWindow.h"
#include "IWindowAddOn.h"

namespace Core {

    class WindowActionFilter;

    class WindowSystemPrivate;

    class IWindowFactoryPrivate {
    public:
        QString id;
        IWindowFactory::AvailableCreator creator;
    };

    class CKAPPCORE_API IWindowPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IWindow)
    public:
        IWindowPrivate();
        ~IWindowPrivate();

        void init();

        void setWindow(QWidget *w, WindowSystemPrivate *d);

        IWindow *q_ptr;

        QString id;
        bool m_closed;

        // mutable bool m_shortcutsDirty;
        QHash<QKeySequence, QPair<QAction *, QWidget *>> shortcutMap;
        QHash<QAction *, QList<QKeySequence>> actionKeyMap;
        WindowActionFilter *actionFilter;

        QMChronMap<QString, ActionItem *> actionItemMap;
        std::list<IWindowAddOn *> addOns;

        QHash<QString, QWidget *> widgetMap;
        QMChronSet<QWidget *> shortcutContextWidgets;

        struct DragFileHandler {
            QObject *obj;
            const char *member;
            int max;
        };
        QHash<QString, DragFileHandler> dragFileHandlerMap;

        void deleteAllAddOns();

        QTimer *delayedInitializeTimer;
        std::list<IWindowAddOn *> delayedInitializeQueue;

        void tryStopDelayedTimer();
        void nextDelayedInitialize();

        void shortcutContextAdded(QWidget *w);
        void shortcutContextRemoved(QWidget *w);
        void addAndFilterAction(QWidget *w, QAction *action);

    private:
        void _q_windowClosed(QWidget *w);
        void _q_actionChanged(QWidget *w, int type, QAction *action);
        void _q_shortcutContextDestroyed(QObject *obj);

        friend class WindowSystem;
    };
}



#endif // IWINDOW_P_H
