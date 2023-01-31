#include "QsApplication_p.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTranslator>

#include "QsSystem.h"

#include "CStartupInfo.h"

static const char Slash = '/';

static QString GetAppConfig() {
    return qApp->applicationName() + "_settings.json";
}

// static QString loadAppleFont() {
//     QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
//     int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
//     QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
//     if (fonts.isEmpty()) {
//         return QString();
//     }
//     return fonts.front();
// }

QsApplicationPrivate::QsApplicationPrivate(QsDistConfig *conf) : conf(conf) {
}

QsApplicationPrivate::~QsApplicationPrivate() {
}

void QsApplicationPrivate::init() {
    Q_Q(QsApplication);

    dd.setDir(qIStup->qsLibPath);
    dd.loadDefault("QsIntegrate");

    // Load or create app config
    if (conf.isNull()) {
        conf.reset(new QsDistConfig());
    }
    conf->initAll();

    // Init managers
    pluginMgr = new QsPluginManager(q);
    fileMgr = new QsFileManager(q);

    conf->load(q->applicationDirPath() + Slash + GetAppConfig());
    if (!conf->apply()) {
        ::exit(-1);
    }

    fileMgr->load();
    pluginMgr->load();
}

void QsApplicationPrivate::deinit() {
    // Save Modules
    pluginMgr->save();
    fileMgr->save();

    delete pluginMgr;
    delete fileMgr;
}
