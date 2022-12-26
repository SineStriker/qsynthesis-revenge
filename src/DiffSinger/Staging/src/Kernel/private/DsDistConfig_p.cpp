#include "DsDistConfig_p.h"

#include <QApplication>
#include <QDebug>
#include <QStandardPaths>

static const char KEY_NAME_EXTENSIONS_DIR[] = "extensions";

static const char Slash = '/';

DsDistConfigPrivate::DsDistConfigPrivate() {
}

DsDistConfigPrivate::~DsDistConfigPrivate() {
}

void DsDistConfigPrivate::init() {
    addInitializer(std::bind(&DsDistConfigPrivate::initByApp, this));
}

void DsDistConfigPrivate::initByApp() {
    setDefaultDir(DsDistConfig::Plugins,                          //
                  QString("${APPPATH}") + Slash + APP_PLUGINS_DIR //
    );

    setDefaultDir(DsDistConfig::BuiltIn,
                  QString("${APPPATH}") + Slash + APP_RES_DIR + Slash + APP_RES_BUILTIN_DIR //
    );

    setDefaultDir(DsDistConfig::BinTool,
                  QString("${APPPATH}") + Slash + APP_TOOLS_DIR //
    );

    setDefaultDir(DsDistConfig::Locales,
                  QString("${APPPATH}") + Slash + APP_LOCALES_DIR //
    );

    registerUserDir(KEY_NAME_EXTENSIONS_DIR,
                    QString("${APPPATH}") + Slash + APP_RES_DIR + Slash + APP_RES_PLUGINS_DIR,
                    DirInitArgs{DirInitArgs::OnlyCheck, {QApplication::addLibraryPath}} //
    );

    setDefault();
}
