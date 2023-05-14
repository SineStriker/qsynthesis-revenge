#include "LastWindowFilter.h"

#include <QEvent>

namespace Core::Internal {

    LastWindowFilter::LastWindowFilter(QObject *parent) : QObject(parent), filtered(false) {
        ICore::instance()->windowSystem()->installEventFilter(this);
    }

    LastWindowFilter::~LastWindowFilter() {
    }

    bool LastWindowFilter::eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::Close) {
            filtered = true;
            ICore::showHome();
            event->ignore();
        }
        return QObject::eventFilter(obj, event);
    }

}
