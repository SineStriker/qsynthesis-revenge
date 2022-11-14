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
    setDefaultDir(LvDistConfig::Plugins,                          //
                  QString("${APPPATH}") + Slash + APP_PLUGINS_DIR //
    );

    setDefaultDir(LvDistConfig::BuiltIn,
                  QString("${APPPATH}") + Slash + APP_RES_DIR + Slash + APP_RES_BUILTIN_DIR //
    );

    registerUserDir(KEY_NAME_EXTENSIONS_DIR,
                    QString("${APPPATH}") + Slash + APP_RES_DIR + Slash + APP_RES_PLUGINS_DIR,
                    DirInitArgs{DirInitArgs::OnlyCheck, {QApplication::addLibraryPath}} //
    );

    setDefault();
}
