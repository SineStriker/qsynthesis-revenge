#include "QsApplication_p.h"

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

static const char Slash = '/';

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

QsApplicationPrivate::QsApplicationPrivate() {
}

QsApplicationPrivate::~QsApplicationPrivate() {
}

void QsApplicationPrivate::init() {
    Q_Q(QsApplication);

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
    tempPath = QDir::tempPath() + Slash + "QSynthTemp";
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

    // Add dll searching path
    // Linux: ./lib
    // Windows: ./lib
#ifdef Q_OS_WINDOWS
    runtimeDir = qApp->applicationDirPath() + "/lib";
    if (!Sys::mkDir(runtimeDir)) {
        QMessageBox::warning(nullptr, q->errorTitle(), QObject::tr("Failed to make runtime path!"));
        ::exit(-1);
    }
    ::SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                               LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_SYSTEM32 |
                               LOAD_LIBRARY_SEARCH_USER_DIRS);
    ::AddDllDirectory(runtimeDir.toStdWString().c_str());
#endif

    // Init managers
    pluginMgr = new PluginManager(q);
    fileMgr = new FileManager(q);
    windowMgr = new WindowManager(q);

    pluginMgr->load();

    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &QsApplication::q_screenRatioChanged);
}

void QsApplicationPrivate::deinit() {
    // Save Modules

    pluginMgr->save();

    delete pluginMgr;
    delete fileMgr;
    delete windowMgr;
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
