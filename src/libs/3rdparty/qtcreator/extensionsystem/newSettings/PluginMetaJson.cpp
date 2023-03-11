#include "PluginMetaJson.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

PluginMetaJson::PluginMetaJson() {
}

PluginMetaJson::PluginMetaJson(const QString &dir) : dir(dir) {
}

PluginMetaJson::~PluginMetaJson() {
}

bool PluginMetaJson::load() {
    QFile file(dir + "/plugin.json");
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

    auto objDoc = doc.object();
    auto it = objDoc.find("name");
    if (it != objDoc.end() && it->isString()) {
        name = it->toString();
    }

    return true;
}

bool PluginMetaJson::save() const {
    return false;
}

QString PluginMetaJson::pluginPath() const {
    QString fileName = QDir::toNativeSeparators(name);

    const bool isAbsolute = QDir::isAbsolutePath(fileName);
    if (isAbsolute) {
        QFileInfo fi(fileName);
        if (fi.isFile()) {
            return fi.canonicalFilePath();
        }
    }

    QStringList prefixes = {"lib"};
    prefixes.prepend(QString());
    QStringList suffixes =
#ifdef Q_OS_WINDOWS
        {".dll"}
#elif defined(Q_OS_MAC)
        {".dylib"}
#else
        {".so"}
#endif
    ;
    suffixes.prepend(QString());

    // Split up "subdir/filename"
    const int slash = fileName.lastIndexOf(QLatin1Char('/'));
    const QStringRef baseName = fileName.midRef(slash + 1);
    const QStringRef basePath = isAbsolute ? QStringRef() : fileName.leftRef(slash + 1); // keep the '/'

    const bool debug = false;

    QStringList paths;
    if (isAbsolute) {
        paths.append(fileName.left(slash)); // don't include the '/'
    } else {
        paths = QStringList(dir);
    }

    for (const QString &path : qAsConst(paths)) {
        for (const QString &prefix : qAsConst(prefixes)) {
            for (const QString &suffix : qAsConst(suffixes)) {
#ifdef Q_OS_ANDROID
                {
                    QString pluginPath = basePath + prefix + baseName + suffix;
                    const QString fn =
                        path + QLatin1String("/lib") + pluginPath.replace(QLatin1Char('/'), QLatin1Char('_'));
                    if (debug)
                        qDebug() << "Trying..." << fn;
                    if (QFileInfo(fn).isFile())
                        return fn;
                }
#endif
                const QString fn = path + QLatin1Char('/') + basePath + prefix + baseName + suffix;
                if (debug)
                    qDebug() << "Trying..." << fn;
                if (QFileInfo(fn).isFile())
                    return fn;
            }
        }
    }
    if (debug)
        qDebug() << fileName << "not found";
    return QString();
}