#include "QsApplication_p.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTranslator>

#include "SystemHelper.h"
#include "ViewHelper.h"

static const char Slash = '/';

static const char FILENAME_APP_CONFIG[] = "qsconfig.json";

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

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    // Load or create app config
    if (conf.isNull()) {
        conf.reset(new QsDistConfig());
    }
    conf->initAll();

    // Init managers
    pluginMgr = new QsPluginManager(q);
    fileMgr = new QsFileManager(q);

    Q_TR_NOTIFY_PRIVATE(QsApplication);
    Q_SS_NOTIFY_PRIVATE(QsApplication);

    conf->load(q->applicationDirPath() + Slash + FILENAME_APP_CONFIG);
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
