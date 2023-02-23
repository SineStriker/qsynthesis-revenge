#include "QMNamespace_p.h"
#include "QMNamespace.h"

#include <QMetaType>

void Register_QMNamespace() {
    qRegisterMetaType<QM::Direction>("QM::Direction");
    qRegisterMetaType<QM::Priority>("QM::Priority");
}
