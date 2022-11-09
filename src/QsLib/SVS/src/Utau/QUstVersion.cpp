#include "QUstVersion.h"

#include "Config/UtaConstants.h"

// Project File Version
QUstVersion::QUstVersion() {
    QUstVersion::clear();
}

QUstVersion::~QUstVersion() {
}

void QUstVersion::reset() {
    version = Utau::UST_VERSION_1_2;
    charset = "";
}

// Project File Notes
bool QUstVersion::operator==(const QUstVersion &another) const {
    return (version == another.version);
}

bool QUstVersion::operator!=(const QUstVersion &another) const {
    return !((*this) == another);
}
