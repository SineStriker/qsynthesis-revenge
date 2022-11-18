#include "LvStartupInfo.h"

LvStartupInfo::LvStartupInfo() {
    appVersion = APP_VERSION;
    appDisplayName = LvStartupInfo::mainTitle();
    parser.setApplicationDescription(QObject::tr("Voice data annotation frontend."));
}

LvStartupInfo::~LvStartupInfo() {
}
