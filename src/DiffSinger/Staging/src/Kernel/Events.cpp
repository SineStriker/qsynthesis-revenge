#include "Events.h"

QEventImpl::WindowCloseEvent::WindowCloseEvent(bool closeOnly)
    : QEvent(static_cast<QEvent::Type>(WindowClose)), c(closeOnly) {
}

QEventImpl::WindowCloseEvent::~WindowCloseEvent() {
}
