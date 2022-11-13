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

static const char FILENAME_APP_CONFIG[] = "qsconf.json";

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

QsApplicationPrivate::QsApplicationPrivate(QsDistConfig *conf) : conf(conf) {
}

QsApplicationPrivate::~QsApplicationPrivate() {
}

void QsApplicationPrivate::init() {
    Q_Q(QsApplication);

    // Load or create app config
    if (conf.isNull()) {
        conf.reset(new QsDistConfig());
    }
    conf->initAll();
    conf->load(q->applicationDirPath() + Slash + FILENAME_APP_CONFIG);

    // Create temporary path
    if (!conf->apply()) {
        ::exit(-1);
    }

    // Init managers
    pluginMgr = new PluginManager(q);
    fileMgr = new FileManager(q);

    fileMgr->load();
    pluginMgr->load();

    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &QsApplication::q_screenRatioChanged);
}

void QsApplicationPrivate::deinit() {
    // Save Modules
    pluginMgr->save();
    fileMgr->save();

    delete pluginMgr;
    delete fileMgr;
}

bool QsApplicationPrivate::translate(const QString &filename) {
    Q_Q(QsApplication);
    QTranslator *t = new QTranslator(q);

    if (t->load(filename)) {
        qApp->installTranslator(t);
        translators.insert(t);
        q->reloadStrings();
        return true;
    }

    delete t;
    return false;
}

void QsApplicationPrivate::eliminate() {
    for (auto it = translators.begin(); it != translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    translators.clear();
}

bool QsApplicationPrivate::addTheme(const QString &filename) {
    Q_Q(QsApplication);

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        stylesheets.append(file.readAll());
        file.close();
        q->reloadScreen();
        return true;
    }

    return false;
}

void QsApplicationPrivate::removeThemes() {
    Q_Q(QsApplication);

    stylesheets.clear();
    q->reloadScreen();
}
