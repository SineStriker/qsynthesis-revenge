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

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

LvElemApplicationPrivate::LvElemApplicationPrivate() {
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

    // Create data path
#ifdef Q_OS_WINDOWS
    dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#elif defined(Q_OS_MAC)
    dataPath =
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/QVogenClient";
#else
    dataPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
#endif
    if (!dataPath.endsWith(q->applicationName())) {
        dataPath += Slash + q->applicationName();
    }
    if (!Sys::mkDir(dataPath)) {
        QMessageBox::warning(nullptr, q->errorTitle(),
                             QObject::tr("Failed to make application data path!"));
        ::exit(-1);
    }

    // Create temporary path
    tempPath = QDir::tempPath() + Slash + q->applicationName();
    if (!Sys::mkDir(tempPath)) {
        QMessageBox::warning(nullptr, q->errorTitle(),
                             QObject::tr("Failed to make temporary path!"));
        ::exit(-1);
    }

    // Setup plugin environment
    pluginDir = qApp->applicationDirPath() + "/" + APP_PLUGINS_DIR;
    if (!Sys::mkDir(pluginDir)) {
        QMessageBox::warning(nullptr, q->errorTitle(), QObject::tr("Failed to make plugin path!"));
        ::exit(-1);
    }
    q->addLibraryPath(pluginDir);

    // Setup extensions environment
    extDir = qApp->applicationDirPath() + "/" + APP_RES_DIR + "/" + APP_RES_PLUGINS_DIR;
    if (!Sys::mkDir(extDir)) {
        QMessageBox::warning(nullptr, q->errorTitle(),
                             QObject::tr("Failed to make extensions path!"));
        ::exit(-1);
    }
    q->addLibraryPath(extDir);

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
