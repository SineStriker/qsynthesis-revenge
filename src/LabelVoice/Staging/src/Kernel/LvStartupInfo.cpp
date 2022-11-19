#include "LvStartupInfo.h"

LvStartupInfo::LvStartupInfo() {
    appVersion = APP_VERSION;
    appDisplayName = LvStartupInfo::mainTitle();
    appDescription = QObject::tr("Voice data annotation tool.");
    parser.addPositionalArgument("files", QObject::tr("Project files to open."),
                                 QObject::tr("[files]"));
}

LvStartupInfo::~LvStartupInfo() {
}
