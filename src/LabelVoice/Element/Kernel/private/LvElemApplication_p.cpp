#include "LvElemApplication_p.h"

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

static const char LVConfig_Filename[] = "lvconf.json";

static bool checkDir(const QString &dir) {
    return Sys::isDirExist(dir);
}

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

LvElemApplicationPrivate::LvElemApplicationPrivate(LvDistConfig *conf) : conf(conf) {
}

LvElemApplicationPrivate::~LvElemApplicationPrivate() {
}

void LvElemApplicationPrivate::init() {
    Q_Q(LvElemApplication);

    // Set app meta data
    q->setApplicationVersion(APP_VERSION);

    // Load codec and fonts
#if defined(Q_OS_WINDOWS)
    auto gbk = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(gbk);

    QString fontName = loadAppleFont();
    QFont f(fontName.isEmpty() ? "Microsoft YaHei" : fontName);
    f.setStyleStrategy(QFont::PreferAntialias);
    f.setPixelSize(12 * (qApp->primaryScreen()->logicalDotsPerInch() / 96.0));
    q->setFont(f);
#endif

    if (conf.isNull()) {
        conf.reset(new LvDistConfig());
    }
    conf->load(q->applicationDirPath() + Slash + LVConfig_Filename);

    // Create data path
    if (!Sys::mkDir(conf->dataPath())) {
        QMessageBox::warning(nullptr, q->errorTitle(),
                             QObject::tr("Failed to make application data path!"));
        ::exit(-1);
    }

    // Create temporary path
    if (!Sys::mkDir(conf->tempPath())) {
        QMessageBox::warning(nullptr, q->errorTitle(),
                             QObject::tr("Failed to make temporary path!"));
        ::exit(-1);
    }

    // Setup qt-plugins environment
    if (!checkDir(conf->pluginDir())) {
        QMessageBox::warning(nullptr, q->errorTitle(), QObject::tr("Failed to make plugin path!"));
        ::exit(-1);
    }
    q->addLibraryPath(conf->pluginDir());

    // Setup qs-plugins environment
    if (!checkDir(conf->builtinDir())) {
        QMessageBox::warning(nullptr, q->errorTitle(), QObject::tr("Failed to make builtin path!"));
        ::exit(-1);
    }
    q->addLibraryPath(conf->builtinDir());

    // Setup lv-plugins environment
    if (!checkDir(conf->extDir())) {
        QMessageBox::warning(nullptr, q->errorTitle(),
                             QObject::tr("Failed to make extensions path!"));
        ::exit(-1);
    }
    q->addLibraryPath(conf->extDir());

    // Init managers
    pluginMgr = new PluginManager(q);
    fileMgr = new FileManager(q);

    fileMgr->load();
    pluginMgr->load();

    // Default translations and styles
    addTheme(":/themes/light/base-light.qss");
    addTheme(":/themes/light/home-light.qss");
    addTheme(":/themes/light/piano-light.qss");

    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &LvElemApplication::q_screenRatioChanged);
}

void LvElemApplicationPrivate::deinit() {
    // Save Modules
    pluginMgr->save();
    fileMgr->save();

    delete pluginMgr;
    delete fileMgr;
}

bool LvElemApplicationPrivate::translate(const QString &filename) {
    Q_Q(LvElemApplication);
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

void LvElemApplicationPrivate::eliminate() {
    for (auto it = translators.begin(); it != translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    translators.clear();
}

bool LvElemApplicationPrivate::addTheme(const QString &filename) {
    Q_Q(LvElemApplication);

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        stylesheets.append(file.readAll());
        file.close();
        q->reloadScreen();
        return true;
    }

    return false;
}

void LvElemApplicationPrivate::removeThemes() {
    Q_Q(LvElemApplication);

    stylesheets.clear();
    q->reloadScreen();
}
