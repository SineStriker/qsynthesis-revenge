#ifndef IWINDOW_P_H
#define IWINDOW_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the ChorusKit API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QHash>
#include <QSet>
#include <QTimer>

#include <QMChronMap.h>
#include <QMChronSet.h>

#include "IWindow.h"
#include "IWindowAddOn.h"

namespace Core {

    class ShortcutContext;

    class WindowCloseFilter;

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
        void closeWindow();

        IWindow *q_ptr;

        QString id;
        bool m_closed;

        WindowCloseFilter *closeFilter;
        ShortcutContext *shortcutContext;

        QMChronMap<QString, ActionItem *> actionItemMap;
        std::list<IWindowAddOn *> addOns;

        QHash<QString, QWidget *> widgetMap;

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

    private:
        void _q_windowClosed(QWidget *w);

        friend class WindowSystem;
    };
}



#endif // IWINDOW_P_H
