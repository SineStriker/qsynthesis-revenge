#include "QsCoreConfig_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>

#include "../QsCoreStartInfo.h"
#include "QsSystem.h"

#include "Serialization/QJsonFormatter.h"

static const char SECTION_NAME_DIR[] = "dir";
static const char KEY_NAME_DATA_DIR[] = "data";
static const char KEY_NAME_TEMP_DIR[] = "temp";
static const char KEY_NAME_SHARE_DIR[] = "share";
static const char KEY_NAME_QT_PLUGINS_DIR[] = "qtplugins";
static const char KEY_NAME_QT_TRANSLATIONS_DIR[] = "translations";
static const char KEY_NAME_QS_PLUGINS_DIR[] = "qsplugins";
static const char KEY_NAME_BINTOOL_DIR[] = "tools";

static const char SECTION_NAME_PLUGINS[] = "plugins";
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

QsCoreConfigPrivate::QsCoreConfigPrivate() {
}

QsCoreConfigPrivate::~QsCoreConfigPrivate() {
}

void QsCoreConfigPrivate::init() {
    addInitializer(std::bind(&QsCoreConfigPrivate::initByApp, this));
}

void QsCoreConfigPrivate::initByApp() {
    /* Set basic environments */
    vars.addHash(QSimpleVarExp::SystemValues());

    /* Set default directories */
    auto wrapDirInfo = [this](const QString &key, const QString &dir, const QString &errorMsg,
                              DirInitArgs::CheckLevel level, bool addLibrary) {
        Q_UNUSED(errorMsg);
        return DirInfo{
            key, vars.parse(dir), dir,
            DirInitArgs{
                level,
                addLibrary
                    ? QList<std::function<void(const QString &)>>{QCoreApplication::addLibraryPath}
                    : QList<std::function<void(const QString &)>>{}}};
    };
    dirMap = {
        {QsCoreConfig::AppData, //
         wrapDirInfo(KEY_NAME_DATA_DIR, QString("${APPDATA}") + Slash + qAppName(), "data",
                     DirInitArgs::CreateIfNotExist, false)},
        {QsCoreConfig::AppTemp, //
         wrapDirInfo(KEY_NAME_TEMP_DIR, QString("${TEMP}") + Slash + qAppName(), "temp",
                     DirInitArgs::CreateIfNotExist, false)},
        {QsCoreConfig::AppShare, //
         wrapDirInfo(KEY_NAME_SHARE_DIR, QString("${APPPATH}") + Slash + "share",
                     "share", DirInitArgs::OnlyCheck, true)},
        {QsCoreConfig::QtPlugins, //
         wrapDirInfo(KEY_NAME_QT_PLUGINS_DIR, QString("${APPPATH}") + Slash + "plugins", "plugins",
                     DirInitArgs::OnlyCheck, true)},
        {QsCoreConfig::QtTranslations, //
         wrapDirInfo(KEY_NAME_QT_TRANSLATIONS_DIR, QString("${APPPATH}") + Slash + "translations",
                     "translations", DirInitArgs::OnlyCheck, true)},
        {QsCoreConfig::QsPlugins, //
         wrapDirInfo(KEY_NAME_QS_PLUGINS_DIR, QString("${APPPATH}") + Slash + "built-in",
                     "built-in", DirInitArgs::OnlyCheck, true)},
        {QsCoreConfig::BinTool, //
         wrapDirInfo(KEY_NAME_BINTOOL_DIR, QString("${APPPATH}") + Slash + "tools", "tools",
                     DirInitArgs::OnlyCheck, true)},
    };
    dirTypeMax = QsCoreConfig::UserDir;

    /* Set default plugins */
    auto wrapPluginInfo = [](const QString &key, const QString &catagory, const QString &name) {
        return PluginInfo{key, catagory, name, name};
    };
    pluginMap = {
        {QsCoreConfig::AudioDecoder, //
         wrapPluginInfo(KEY_NAME_AUDIO_DECODER, "audiodecoders", DEFAULT_AUDIO_DECODER)},
        {QsCoreConfig::AudioEncoder, //
         wrapPluginInfo(KEY_NAME_AUDIO_ENCODER, "audioencoders", DEFAULT_AUDIO_DECODER)},
        {QsCoreConfig::AudioPlayback, //
         wrapPluginInfo(KEY_NAME_PLAYBACK, "audioplaybacks", DEFAULT_PLAYBACK)},
        {QsCoreConfig::CompressEngine, //
         wrapPluginInfo(KEY_NAME_COMPRESS_ENGINE, "compressengines", DEFAULT_COMPRESS_ENGINE)},
        {QsCoreConfig::WindowFactory, //
         wrapPluginInfo(KEY_NAME_WINDOW_FACTORY, "windowfactories", DEFAULT_WINDOW_FACTORY)},
    };
    pluginTypeMax = QsCoreConfig::UserPlugin;
}

bool QsCoreConfigPrivate::load_helper(const QString &filename) {
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
            info.dir = vars.parse(QDir::fromNativeSeparators(it3.value().toString()));
        }
    }

    // Get plugins
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

bool QsCoreConfigPrivate::apply_helper() {
    for (auto it = dirMap.begin(); it != dirMap.end(); ++it) {
        const auto &info = it.value();
        const auto &args = info.initArgs;
        QString path = info.dir;
        switch (args.level) {
            case DirInitArgs::CreateIfNotExist:
                if (!QsSys::mkDir(path)) {
                    QsSys::osMessageStderr(qIStup->errorTitle(),
                                           QString("Failed to create %1 directory!")
                                               .arg(QsSys::PathFindFileName(path)));
                    return false;
                }
                break;
            case DirInitArgs::ErrorIfNotExist:
                if (!QsSys::isDirExist(path)) {
                    QsSys::osMessageStderr(
                        qIStup->errorTitle(),
                        QString("Failed to find %1 directory!").arg(QsSys::PathFindFileName(path)));
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

bool QsCoreConfigPrivate::save_default(const QString &filename) {
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

void QsCoreConfigPrivate::setDefault() {
    for (auto it = dirMap.begin(); it != dirMap.end(); ++it) {
        auto &info = it.value();
        info.dir = vars.parse(info.defaultDir);
    }
    for (auto it = pluginMap.begin(); it != pluginMap.end(); ++it) {
        auto &info = it.value();
        info.name = info.defaultName;
    }
}

void QsCoreConfigPrivate::registerEscapeVar(const QString &key, const QString &val, bool replace) {
    auto it = vars.Variables.find(key);
    if (it != vars.Variables.end()) {
        if (replace) {
            it.value() = val;
        }
    } else {
        vars.Variables.insert(key, val);
    }
}

int QsCoreConfigPrivate::registerUserDir(const QString &key, const QString &dir,
                                         const DirInitArgs &args, int hint) {
    dirTypeMax = (hint > dirTypeMax) ? hint : (dirTypeMax + 1);
    dirMap.insert(dirTypeMax, DirInfo{key, dir, dir, args});
    return dirTypeMax;
}

int QsCoreConfigPrivate::registerUserPlugin(const QString &key, const QString &catagory,
                                            const QString &name, int hint) {
    pluginTypeMax = (hint > pluginTypeMax) ? hint : (pluginTypeMax + 1);
    pluginMap.insert(pluginTypeMax, PluginInfo{key, catagory, name, name});
    return pluginTypeMax;
}

void QsCoreConfigPrivate::setDefaultDir(int type, const QString &dir) {
    auto it = dirMap.find(type);
    if (it == dirMap.end()) {
        return;
    }
    DirInfo &info = it.value();
    info.defaultDir = dir;
}

void QsCoreConfigPrivate::setDefaultPluginName(int type, const QString &dir) {
    auto it = pluginMap.find(type);
    if (it == pluginMap.end()) {
        return;
    }
    PluginInfo &info = it.value();
    info.defaultName = dir;
}

void QsCoreConfigPrivate::addInitializer(const std::function<void()> &fun) {
    this->initializers.append(fun);
}
