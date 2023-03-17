#ifndef IWINDOW_P_H
#define IWINDOW_P_H

#include <QSet>

#include "IWindow.h"
#include "IWindowAddOn.h"

namespace Core {

    class IWindowFactoryPrivate {
    public:
        QString id;
        IWindowFactory::AvailableCreator creator;
    };

    class IWindowPrivate : public QObject {
        Q_OBJECT
    public:
        IWindowPrivate();

        void init();

        IWindow *q_ptr;

        QString id;
        QWidget *window;

        QMap<QString, ActionItem *> actionItemMap;
        std::list<IWindowAddOn *> addOns;

        void initAllAddOns();
        void deleteAllAddOns();

        void _q_windowClosed(QWidget *w);
    };
}



#endif // IWINDOW_P_H
