#include "Events_p.h"

#include "../Events.h"

void Register_Events() {
    for (auto i = QEvent::User + 1; i < QEventImpl::NumUserEvent; ++i) {
        QEvent::registerEventType(i);
    }
}
