#include "CVisualTypes.h"

#include <QMetaType>

void CV::Register() {
    qRegisterMetaType<CV::Direction>("CV::Direction");
    qRegisterMetaType<CV::Priority>("CV::Priority");
}
