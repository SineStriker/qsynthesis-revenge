#include "LvDistConfig_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>

#include "Serialization/QJsonFormatter.h"

static const char SECTION_NAME_DIR[] = "dir";
static const char KEY_NAME_DATA_DIR[] = "data";
static const char KEY_NAME_TEMP_DIR[] = "temp";
static const char KEY_NAME_PLUGIN_DIR[] = "plugin";
static const char KEY_NAME_BUILTIN_DIR[] = "builtin";
static const char KEY_NAME_EXTENSIONS_DIR[] = "extensions";

static const char SECTION_NAME_BUILTIN[] = "default-plugins";
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

LvDistConfigPrivate::LvDistConfigPrivate() {
}

LvDistConfigPrivate::~LvDistConfigPrivate() {
}

void LvDistConfigPrivate::init() {
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

    setDefault();
}

bool LvDistConfigPrivate::load_helper(const QString &filename) {
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

    /* Default dirs */
    QString dataDir;
    QString tempDir;
    QString pluginDir;
    QString builtinDir;
    QString extDir;

    /* Default builtin plugins */
    QString encoder;
    QString decoder;
    QString compression;
    QString playback;
    QString windowFac;

    QJsonFormatter f;
    bool parsed =
        f.parse(doc.object(),
                f.OT({{SECTION_NAME_DIR, f.OT({{KEY_NAME_DATA_DIR, f.S(&dataDir)},
                                               {KEY_NAME_TEMP_DIR, f.S(&tempDir)},
                                               {KEY_NAME_PLUGIN_DIR, f.S(&pluginDir)},
                                               {KEY_NAME_BUILTIN_DIR, f.S(&builtinDir)},
                                               {KEY_NAME_EXTENSIONS_DIR, f.S(&extDir)}})},
                      {SECTION_NAME_BUILTIN, f.OT({{KEY_NAME_AUDIO_DECODER, f.S(&decoder)},
                                                   {KEY_NAME_AUDIO_ENCODER, f.S(&encoder)},
                                                   {KEY_NAME_COMPRESS_ENGINE, f.S(&compression)},
                                                   {KEY_NAME_PLAYBACK, f.S(&playback)},
                                                   {KEY_NAME_WINDOW_FACTORY, f.S(&windowFac)}})}}));
    if (!parsed) {
        return false;
    }

    this->dataDir = parse(dataDir);
    this->tempDir = parse(tempDir);
    this->pluginDir = parse(pluginDir);
    this->builtinDir = parse(builtinDir);
    this->extDir = parse(extDir);

    this->decoder = parse(decoder);
    this->encoder = parse(encoder);
    this->compression = parse(compression);
    this->playback = parse(playback);
    this->windowFac = parse(windowFac);

    return true;
}

bool LvDistConfigPrivate::save_default(const QString &filename) {
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    QJsonDocument doc;
    QJsonObject obj{
        {SECTION_NAME_DIR,
         QJsonObject{
             {KEY_NAME_DATA_DIR, //
              QString("${APPDATA}") + Slash + qAppName()},
             {KEY_NAME_TEMP_DIR, //
              QString("${TEMP}") + Slash + qAppName()},
             {KEY_NAME_PLUGIN_DIR, //
              QString("${APPPATH}") + Slash + APP_PLUGINS_DIR},
             {KEY_NAME_BUILTIN_DIR,
              QString("${APPPATH}") + Slash + +APP_RES_DIR + Slash + APP_RES_BUILTIN_DIR},
             {KEY_NAME_EXTENSIONS_DIR,
              QString("${APPPATH}") + Slash + APP_RES_DIR + Slash + APP_RES_PLUGINS_DIR},
         }},
        {SECTION_NAME_BUILTIN, QJsonObject{{KEY_NAME_AUDIO_DECODER, DEFAULT_AUDIO_DECODER},
                                           {KEY_NAME_AUDIO_ENCODER, DEFAULT_AUDIO_ENCODER},
                                           {KEY_NAME_COMPRESS_ENGINE, DEFAULT_COMPRESS_ENGINE},
                                           {KEY_NAME_PLAYBACK, DEFAULT_PLAYBACK},
                                           {KEY_NAME_WINDOW_FACTORY, DEFAULT_WINDOW_FACTORY}}

        },
    };
    doc.setObject(obj);
    file.write(doc.toJson());
    file.close();

    return true;
}

void LvDistConfigPrivate::setDefault() {
    dataDir = GetAppDataPath() + Slash + qAppName();
    tempDir = QDir::tempPath() + Slash + qAppName();
    pluginDir = qApp->applicationDirPath() + Slash + APP_PLUGINS_DIR;
    builtinDir = qApp->applicationDirPath() + Slash + APP_RES_DIR + Slash + APP_RES_BUILTIN_DIR;
    extDir = qApp->applicationDirPath() + Slash + APP_RES_DIR + Slash + APP_RES_PLUGINS_DIR;

    decoder = DEFAULT_AUDIO_DECODER;
    encoder = DEFAULT_AUDIO_ENCODER;
    compression = DEFAULT_COMPRESS_ENGINE;
    playback = DEFAULT_PLAYBACK;
    windowFac = DEFAULT_WINDOW_FACTORY;
}

QString LvDistConfigPrivate::parse(const QString &str) const {
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
