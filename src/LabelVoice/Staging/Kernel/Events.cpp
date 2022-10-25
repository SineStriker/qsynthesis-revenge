#include "Events.h"

void QEventImpl::Register() {
    for (auto i = QEvent::User + 1; i < NumUserEvent; ++i) {
        QEvent::registerEventType(i);
    }
}

QEventImpl::WindowCloseEvent::WindowCloseEvent(bool closeOnly)
    : QEvent(static_cast<QEvent::Type>(WindowClose)), c(closeOnly) {
}

QEventImpl::WindowCloseEvent::~WindowCloseEvent() {
}
