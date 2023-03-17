#include "WindowCloseFilter_p.h"

#include <QEvent>

namespace Core {

    WindowCloseFilter::WindowCloseFilter(QWidget *w) : QObject(w), w(w) {
        w->installEventFilter(this);
    }

    WindowCloseFilter::~WindowCloseFilter() {
    }

    bool WindowCloseFilter::eventFilter(QObject *obj, QEvent *event) {
        if (obj == w) {
            switch (event->type()) {
                case QEvent::Close:
                    if (event->isAccepted()) {
                        emit windowClosed(w);
                    }
                    break;
                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
    }

}