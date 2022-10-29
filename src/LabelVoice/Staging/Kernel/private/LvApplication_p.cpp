#include "LvApplication_p.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTranslator>

#include "Kernel/Events.h"
#include "SystemHelper.h"
#include "ViewHelper.h"

#include "Styles/QCssAnalyzer.h"

static const char Slash = '/';

LvApplicationPrivate::LvApplicationPrivate() {
}

LvApplicationPrivate::~LvApplicationPrivate() {
}

void LvApplicationPrivate::init() {
    Q_Q(LvApplication);

    // Set app meta data
    q->setApplicationVersion(APP_VERSION);

    // Register user types
    QEventImpl::Register();

    // Load codec and fonts
#if defined(Q_OS_WINDOWS)
    auto gbk = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(gbk);

    QFont f("Microsoft YaHei");
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
    pluginDir = qApp->applicationDirPath() + "/plugins";
    if (!Sys::mkDir(pluginDir)) {
        QMessageBox::warning(nullptr, q->errorTitle(), QObject::tr("Failed to make plugin path!"));
        ::exit(-1);
    }
    q->addLibraryPath(pluginDir);

    // Init managers
    pluginMgr = new PluginManager(q);
    fileMgr = new FileManager(q);
    windowMgr = new WindowManager(q);

    fileMgr->load();
    pluginMgr->load();

    // Default translations and styles
    addTheme(":/themes/light/base-light.qss");
    addTheme(":/themes/light/home-light.qss");
    addTheme(":/themes/light/piano-light.qss");

    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &LvApplication::q_screenRatioChanged);
}

void LvApplicationPrivate::deinit() {
    // Save Modules

    pluginMgr->save();
    fileMgr->save();

    delete pluginMgr;
    delete fileMgr;
    delete windowMgr;
}

void LvApplicationPrivate::instanceStarted_helper() {
}

void LvApplicationPrivate::messageReceived_helper(const QStringList &args) {
    View::bringWindowToForeground(windowMgr->firstWindow());

    Q_UNUSED(args);
}

bool LvApplicationPrivate::translate(const QString &filename) {
    Q_Q(LvApplication);
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

void LvApplicationPrivate::eliminate() {
    for (auto it = translators.begin(); it != translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    translators.clear();
}

bool LvApplicationPrivate::addTheme(const QString &filename) {
    Q_Q(LvApplication);

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        stylesheets.append(file.readAll());
        file.close();
        q->reloadScreen();
        return true;
    }

    return false;
}

void LvApplicationPrivate::removeThemes() {
    Q_Q(LvApplication);

    stylesheets.clear();
    q->reloadScreen();
}
