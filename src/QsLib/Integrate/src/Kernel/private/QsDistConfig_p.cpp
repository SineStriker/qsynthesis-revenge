#include "QsDistConfig_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>

#include "../QsApplication.h"
#include "SystemHelper.h"

#include "Serialization/QJsonFormatter.h"

static const char SECTION_NAME_DIR[] = "dir";
static const char KEY_NAME_DATA_DIR[] = "data";
static const char KEY_NAME_TEMP_DIR[] = "temp";
static const char KEY_NAME_PLUGIN_DIR[] = "plugin";
static const char KEY_NAME_BUILTIN_DIR[] = "builtin";
static const char KEY_NAME_BINTOOL_DIR[] = "tools";

static const char SECTION_NAME_PLUGINS[] = "default-plugins";
static const char KEY_NAME_AUDIO_DECODER[] = "audioDecoder";
static const char KEY_NAME_AUDIO_ENCODER[] = "audioEncoder";
static const char KEY_NAME_COMPRESS_ENGINE[] = "compression";
static const char KEY_NAME_PLAYBACK[] = "audioPlayback";
static const char KEY_NAME_WINDOW_FACTORY[] = "windowFactory";

static const char DEFAULT_AUDIO_DECODER[] = "FFmpegDecoder";
static const char DEFAULT_AUDIO_ENCODER[] = "FFmpegEncoder";
static const char DEFAULT_COMPRESS_ENGINE[] = "qzlib";
static const char DEFAULT_PLAYBACK[] = "SDLPlayback";
static const char DEFAULT_WINDOW_FACTORY[] = "NativeWindow";

static const char Slash = '/';

static QString GetAppDataPath() {
    QString path;
#ifdef Q_OS_WINDOWS
    path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#elif defined(Q_OS_MAC)
    path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config";
#else
    path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
#endif
    QString slashName = Slash + qAppName();
    if (path.endsWith(slashName)) {
        path.chop(slashName.size());
    }
    return path;
}

QsDistConfigPrivate::QsDistConfigPrivate() {
}

QsDistConfigPrivate::~QsDistConfigPrivate() {
}

void QsDistConfigPrivate::init() {
    addInitializer(std::bind(&QsDistConfigPrivate::initByApp, this));
}

void QsDistConfigPrivate::initByApp() {
    /* Set basic environments */
    vars = {
        {"DESKTOP", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)},
        {"DOCUMENTS", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)},
        {"APPLICATIONS", QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)},
        {"HOME", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)},
        {"APPDATA", GetAppDataPath()},
        {"TEMP", QDir::tempPath()},
        {"ROOT", QDir::rootPath()},
        {"APPPATH", qApp->applicationDirPath()},
        {"APPNAME", qAppName()},
    };

    /* Set default directories */
    auto wrapDirInfo = [this](const QString &key, const QString &dir, const QString &errorMsg,
                              DirInitArgs::CheckLevel level, bool addLibrary) {
        Q_UNUSED(errorMsg);
        return DirInfo{
            key, parse(dir), dir,
            DirInitArgs{
                level,
                addLibrary
                    ? QList<std::function<void(const QString &)>>{QApplication::addLibraryPath}
                    : QList<std::function<void(const QString &)>>{}}};
    };
    dirMap = {
        {QsDistConfig::AppData, //
         wrapDirInfo(KEY_NAME_DATA_DIR, QString("${APPDATA}") + Slash + qAppName(), "data",
                     DirInitArgs::CreateIfNotExist, false)},
        {QsDistConfig::AppTemp, //
         wrapDirInfo(KEY_NAME_TEMP_DIR, QString("${TEMP}") + Slash + qAppName(), "temp",
                     DirInitArgs::CreateIfNotExist, false)},
        {QsDistConfig::Plugins, //
         wrapDirInfo(KEY_NAME_PLUGIN_DIR, QString("${APPPATH}") + Slash + "plugins", "plugins",
                     DirInitArgs::OnlyCheck, true)},
        {QsDistConfig::BuiltIn, //
         wrapDirInfo(KEY_NAME_BUILTIN_DIR, QString("${APPPATH}") + Slash + "built-in", "built-in",
                     DirInitArgs::OnlyCheck, true)},
        {QsDistConfig::BinTool, //
         wrapDirInfo(KEY_NAME_BINTOOL_DIR, QString("${APPPATH}") + Slash + "tools", "tools",
                     DirInitArgs::OnlyCheck, true)},
    };
    dirTypeMax = QsDistConfig::UserDir;

    /* Set default plugins */
    auto wrapPluginInfo = [](const QString &key, const QString &catagory, const QString &name) {
        return PluginInfo{key, catagory, name, name};
    };
    pluginMap = {
        {QsDistConfig::AudioDecoder, //
         wrapPluginInfo(KEY_NAME_AUDIO_DECODER, "audiodecoders", DEFAULT_AUDIO_DECODER)},
        {QsDistConfig::AudioEncoder, //
         wrapPluginInfo(KEY_NAME_AUDIO_ENCODER, "audioencoders", DEFAULT_AUDIO_DECODER)},
        {QsDistConfig::AudioPlayback, //
         wrapPluginInfo(KEY_NAME_PLAYBACK, "audioplaybacks", DEFAULT_PLAYBACK)},
        {QsDistConfig::CompressEngine, //
         wrapPluginInfo(KEY_NAME_COMPRESS_ENGINE, "compressengines", DEFAULT_COMPRESS_ENGINE)},
        {QsDistConfig::WindowFactory, //
         wrapPluginInfo(KEY_NAME_WINDOW_FACTORY, "windowfactories", DEFAULT_WINDOW_FACTORY)},
    };
    pluginTypeMax = QsDistConfig::UserPlugin;
}

bool QsDistConfigPrivate::load_helper(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data(file.readAll());
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    QJsonObject objDoc = doc.object();

    QJsonObject::ConstIterator it;

    // Get dirs
    it = objDoc.find(SECTION_NAME_DIR);
    if (it != objDoc.end() && it.value().isObject()) {
        QJsonObject obj = it.value().toObject();
        for (auto it2 = dirMap.begin(); it2 != dirMap.end(); ++it2) {
            DirInfo &info = it2.value();
            auto it3 = obj.find(info.key);
            if (it3 == obj.end() || !it3.value().isString()) {
                continue;
            }
            info.dir = parse(QDir::fromNativeSeparators(it3.value().toString()));
        }
    }

    // Get plgins
    it = objDoc.find(SECTION_NAME_PLUGINS);
    if (it != objDoc.end() && it.value().isObject()) {
        QJsonObject obj = it.value().toObject();
        for (auto it2 = pluginMap.begin(); it2 != pluginMap.end(); ++it2) {
            PluginInfo &info = it2.value();
            auto it3 = obj.find(info.key);
            if (it3 == obj.end() || !it3.value().isString()) {
                continue;
            }
            info.name = it3.value().toString();
        }
    }

    return true;
}

bool QsDistConfigPrivate::apply_helper() {
    for (auto it = dirMap.begin(); it != dirMap.end(); ++it) {
        const auto &info = it.value();
        const auto &args = info.initArgs;
        QString path = info.dir;
        switch (args.level) {
            case DirInitArgs::CreateIfNotExist:
                if (!Sys::mkDir(path)) {
                    QMessageBox::critical(
                        nullptr, qApp->errorTitle(),
                        QString("Failed to create %1 directory!").arg(Sys::PathFindFileName(path)));
                    return false;
                }
                break;
            case DirInitArgs::ErrorIfNotExist:
                if (!Sys::isDirExist(path)) {
                    QMessageBox::critical(
                        nullptr, qApp->errorTitle(),
                        QString("Failed to find %1 directory!").arg(Sys::PathFindFileName(path)));
                    return false;
                }
                break;
            default:
                break;
        }
        // Traverse dir functions
        for (auto it = args.initializers.begin(); it != args.initializers.end(); ++it) {
            (*it)(path);
        }
    }
    return true;
}

bool QsDistConfigPrivate::save_default(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    QJsonObject dirsObj;
    for (auto it = dirMap.begin(); it != dirMap.end(); ++it) {
        const auto &info = it.value();
        dirsObj.insert(info.key, info.defaultDir);
    }

    QJsonDocument doc;
    QJsonObject obj{
        {SECTION_NAME_DIR, dirsObj},
        {
            SECTION_NAME_PLUGINS,
            QJsonObject{{KEY_NAME_AUDIO_DECODER, DEFAULT_AUDIO_DECODER},
                        {KEY_NAME_AUDIO_ENCODER, DEFAULT_AUDIO_ENCODER},
                        {KEY_NAME_COMPRESS_ENGINE, DEFAULT_COMPRESS_ENGINE},
                        {KEY_NAME_PLAYBACK, DEFAULT_PLAYBACK},
                        {KEY_NAME_WINDOW_FACTORY, DEFAULT_WINDOW_FACTORY}},
        },
    };
    doc.setObject(obj);
    file.write(doc.toJson());
    file.close();

    return true;
}

void QsDistConfigPrivate::setDefault() {
    for (auto it = dirMap.begin(); it != dirMap.end(); ++it) {
        auto &info = it.value();
        info.dir = parse(info.defaultDir);
    }
    for (auto it = pluginMap.begin(); it != pluginMap.end(); ++it) {
        auto &info = it.value();
        info.name = info.defaultName;
    }
}

QString QsDistConfigPrivate::parse(const QString &str) const {
    struct Result {
        QString key;
        QString val;
        QString full;
    };
    QList<Result> tempList;

    QRegularExpression reg("\\$\\{(\\w+)\\}");
    auto it = reg.globalMatch(str);
    while (it.hasNext()) {
        auto match = it.next();
        QString var = match.captured(1);
        auto it2 = vars.find(var);
        if (it2 != vars.end()) {
            tempList.append(Result{var, it2.value(), match.captured()});
        }
    }

    QString str2 = str;
    for (const Result &res : qAsConst(tempList)) {
        str2.replace(res.full, res.val);
    }
    return str2;
}

void QsDistConfigPrivate::registerEscapeVar(const QString &key, const QString &val, bool replace) {
    auto it = vars.find(key);
    if (it != vars.end()) {
        if (replace) {
            it.value() = val;
        }
    } else {
        vars.insert(key, val);
    }
}

int QsDistConfigPrivate::registerUserDir(const QString &key, const QString &dir,
                                         const DirInitArgs &args, int hint) {
    dirTypeMax = (hint > dirTypeMax) ? hint : (dirTypeMax + 1);
    dirMap.insert(dirTypeMax, DirInfo{key, dir, dir, args});
    return dirTypeMax;
}

int QsDistConfigPrivate::registerUserPlugin(const QString &key, const QString &catagory,
                                            const QString &name, int hint) {
    pluginTypeMax = (hint > pluginTypeMax) ? hint : (pluginTypeMax + 1);
    pluginMap.insert(pluginTypeMax, PluginInfo{key, catagory, name, name});
    return pluginTypeMax;
}

void QsDistConfigPrivate::setDefaultDir(int type, const QString &dir) {
    auto it = dirMap.find(type);
    if (it == dirMap.end()) {
        return;
    }
    DirInfo &info = it.value();
    info.defaultDir = dir;
}

void QsDistConfigPrivate::setDefaultPluginName(int type, const QString &dir) {
    auto it = pluginMap.find(type);
    if (it == pluginMap.end()) {
        return;
    }
    PluginInfo &info = it.value();
    info.defaultName = dir;
}

void QsDistConfigPrivate::addInitializer(const std::function<void()> &fun) {
    this->initializers.append(fun);
}
