#include "QUtaDistConfig_p.h"

#include <QApplication>
#include <QDebug>
#include <QStandardPaths>

static const char KEY_NAME_EXTENSIONS_DIR[] = "extensions";

static const char Slash = '/';

QUtaDistConfigPrivate::QUtaDistConfigPrivate() {
}

QUtaDistConfigPrivate::~QUtaDistConfigPrivate() {
}

void QUtaDistConfigPrivate::init() {
    addInitializer(std::bind(&QUtaDistConfigPrivate::initByApp, this));
}

void QUtaDistConfigPrivate::initByApp() {
    setDefaultDir(QUtaDistConfig::Plugins,                                //
                  QString("${APPPATH}") + Slash + APP_PRIVATE_PLUGINS_DIR //
    );

    setDefaultDir(QUtaDistConfig::BuiltIn,
                  QString("${APPPATH}") + Slash + APP_PUBLIC_PLUGINS_DIR //
    );

    setDefaultDir(QUtaDistConfig::BinTool,
                  QString("${APPPATH}") + Slash + APP_TOOLS_DIR //
    );

    setDefaultDir(QUtaDistConfig::Locales,
                  QString("${APPPATH}") + Slash + APP_LOCALES_DIR //
    );

    registerUserDir(KEY_NAME_EXTENSIONS_DIR,
                    QString("${APPPATH}") + Slash + APP_EXT_DIR + Slash + APP_RES_PLUGINS_DIR,
                    DirInitArgs{DirInitArgs::OnlyCheck, {QApplication::addLibraryPath}} //
    );

    setDefault();
}
