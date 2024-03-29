#include "LvDistConfig_p.h"

#include <QApplication>
#include <QDebug>
#include <QStandardPaths>

static const char KEY_NAME_EXTENSIONS_DIR[] = "extensions";

static const char Slash = '/';

LvDistConfigPrivate::LvDistConfigPrivate() {
}

LvDistConfigPrivate::~LvDistConfigPrivate() {
}

void LvDistConfigPrivate::init() {
    addInitializer(std::bind(&LvDistConfigPrivate::initByApp, this));
}

void LvDistConfigPrivate::initByApp() {
    setDefaultDir(LvDistConfig::Plugins,                                  //
                  QString("${APPPATH}") + Slash + APP_PRIVATE_PLUGINS_DIR //
    );

    setDefaultDir(LvDistConfig::BuiltIn,
                  QString("${APPPATH}") + Slash + APP_PUBLIC_PLUGINS_DIR //
    );

    setDefaultDir(LvDistConfig::BinTool,
                  QString("${APPPATH}") + Slash + APP_TOOLS_DIR //
    );

    setDefaultDir(LvDistConfig::Locales,
                  QString("${APPPATH}") + Slash + APP_LOCALES_DIR //
    );

    registerUserDir(KEY_NAME_EXTENSIONS_DIR,
                    QString("${APPPATH}") + Slash + APP_EXT_DIR + Slash + APP_RES_PLUGINS_DIR,
                    DirInitArgs{DirInitArgs::OnlyCheck, {QApplication::addLibraryPath}} //
    );

    setDefault();
}
