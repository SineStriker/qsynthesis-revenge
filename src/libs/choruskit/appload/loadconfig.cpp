#include "loadconfig.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

bool LoadConfig::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray data(file.readAll());
    file.close();

    QJsonParseError err{};
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    auto objDoc = doc.object();
    {
        auto it = objDoc.find("AppName");
        if (it != objDoc.end() && it->isString()) {
            appName = it->toString();
        }
    }

    {
        auto it = objDoc.find("AppVersion");
        if (it != objDoc.end() && it->isString()) {
            appVersion = it->toString();
        }
    }

    {
        auto it = objDoc.find("AppOrganizationName");
        if (it != objDoc.end() && it->isString()) {
            appOrganizationName = it->toString();
        }
    }

    {
        auto it = objDoc.find("AppOrganizationDomain");
        if (it != objDoc.end() && it->isString()) {
            appOrganizationDomain = it->toString();
        }
    }

    {
        auto it = objDoc.find("PluginIID");
        if (it != objDoc.end() && it->isString()) {
            pluginIID = it->toString();
        }
    }

    {
        auto it = objDoc.find("SplashImage");
        if (it != objDoc.end() && it->isString()) {
            splashImage = it->toString();
        }
    }

    {
        auto it = objDoc.find("CoreName");
        if (it != objDoc.end() && it->isString()) {
            coreName = it->toString();
        }
    }

    {
        auto it = objDoc.find("ResourceFiles");
        if (it != objDoc.end() && it->isArray()) {
            auto arr = it->toArray();
            for (const auto &item : qAsConst(arr)) {
                if (item.isString()) {
                    resourceFiles.append(item.toString());
                }
            }
        }
    }

    return true;
}
