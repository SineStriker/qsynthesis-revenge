#ifndef EVENTS_H
#define EVENTS_H

#include <QEvent>

namespace QEventImpl {

    enum Event {

        WindowClose = QEvent::User + 1,

        NumUserEvent,

    };

    class WindowCloseEvent : public QEvent {
    public:
        WindowCloseEvent(bool closeOnly = false);
        ~WindowCloseEvent();

        inline bool closeOnly() const {
            return c;
        }

    protected:
        bool c;
    };

}

#endif // EVENTS_H
