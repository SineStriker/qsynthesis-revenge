#include "QsNamespace_p.h"

#include "../QsNamespace.h"

#include <QMetaType>

void Register_QsNamespace() {
    qRegisterMetaType<Qs::Direction>("Qs::Direction");
    qRegisterMetaType<Qs::Priority>("Qs::Priority");
}
