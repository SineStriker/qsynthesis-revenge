#ifndef IWINDOW_P_H
#define IWINDOW_P_H

#include <QHash>
#include <QSet>

#include "IWindow.h"
#include "IWindowAddOn.h"

namespace Core {

    class IWindowFactoryPrivate {
    public:
        QString id;
        IWindowFactory::AvailableCreator creator;
    };

    class CORE_EXPORT IWindowPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IWindow)
    public:
        IWindowPrivate();
        ~IWindowPrivate();

        void init();

        IWindow *q_ptr;

        QString id;
        QWidget *window;

        QMap<QString, ActionItem *> actionItemMap;
        std::list<IWindowAddOn *> addOns;

        QMap<QString, QWidget *> widgetMap;

        QMap<QString, QSet<QObject *>> objectMap;
        QHash<QObject *, QString> objectIndexes;
        mutable QReadWriteLock objectListLock;

        void initAllAddOns();
        void deleteAllAddOns();

        void _q_windowClosed(QWidget *w);
    };
}



#endif // IWINDOW_P_H
