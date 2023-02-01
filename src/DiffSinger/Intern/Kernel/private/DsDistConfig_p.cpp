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
    setDefaultDir(DsDistConfig::AppShare,
                  QString("${APPPATH}") + Slash + APP_SHARE_DIR //
    );

    setDefaultDir(DsDistConfig::QtPlugins,
                  QString("${APPPATH}") + Slash + APP_PLUGINS_DIR //
    );

    setDefaultDir(DsDistConfig::QtTranslations,
                  QString("${APPPATH}") + Slash + APP_LOCALES_DIR //
    );

    setDefaultDir(DsDistConfig::QsPlugins,
                  QString("${APPPATH}") + Slash + APP_PUBLIC_PLUGINS_DIR //
    );

    setDefaultDir(DsDistConfig::BinTool,
                  QString("${APPPATH}") + Slash + APP_TOOLS_DIR //
    );

    registerUserDir(KEY_NAME_EXTENSIONS_DIR,
                    QString("${APPPATH}") + Slash + APP_PRIVATE_PLUGINS_DIR,
                    DirInitArgs{DirInitArgs::OnlyCheck, {QApplication::addLibraryPath}} //
    );

    setDefault();
}
