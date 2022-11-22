#include "QsNamespace.h"

#include <QMetaType>

static bool isRegistered = false;

void Qs::Register() {
    Q_ASSERT(!isRegistered);
    isRegistered = true;

    qRegisterMetaType<Qs::Direction>("Qs::Direction");
    qRegisterMetaType<Qs::Priority>("Qs::Priority");
}
