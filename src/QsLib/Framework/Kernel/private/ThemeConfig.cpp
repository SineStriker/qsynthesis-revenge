#include "ThemeConfig.h"

#include "QsCss.h"

#include "QsLinq.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

static const char Theme_Config_Key_Config[] = "config";
static const char Theme_Config_Key_Sizes[] = "sizes";
static const char Theme_Config_Key_Colors[] = "colors";
static const char Theme_Config_Key_Separator[] = ".";

static const char Theme_Config_Key_Config_Priority[] = "priority";

ThemeConfig::ThemeConfig() {
    priority = 1;
}

ThemeConfig::~ThemeConfig() {
}

bool ThemeConfig::load(const QStringList &filenames) {
    return !QsLinq::Any(filenames, [this](const QString &filename) { return !loadOne(filename); });
}

bool ThemeConfig::loadOne(const QString &filename) {
    QFile file(filename);

    // Open file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    file.close();

    if (err.error != QJsonParseError::NoError) {
        qDebug().noquote() << "ThemeConfig: Json parse error" << err.errorString();
        return false;
    }

    if (!doc.isObject()) {
        qDebug().noquote() << "ThemeConfig: Not a json object";
        return false;
    }

    QJsonObject objDoc = doc.object();

    // Get config
    while (true) {
        std::list<QPair<QStringList, QJsonObject>> stack;
        auto it = objDoc.find(Theme_Config_Key_Config);
        if (it == objDoc.end() || !it.value().isObject()) {
            qDebug().noquote() << "ThemeConfig: Missing configuration";
            return false;
        }

        auto obj = it.value().toObject();

        it = obj.find(Theme_Config_Key_Config_Priority);
        if (it != objDoc.end() && it.value().isDouble()) {
            priority = it.value().toDouble();
        }
        break;
    }

    // Get colors
    {
        std::list<QPair<QStringList, QJsonObject>> stack;
        {
            auto it = objDoc.find(Theme_Config_Key_Colors);
            if (it != objDoc.end() && it.value().isObject()) {
                stack.push_back(qMakePair(QStringList(), it.value().toObject()));
            }
        }

        while (!stack.empty()) {
            auto pair = stack.front();
            stack.pop_front();

            const auto &keys = pair.first;
            const auto &obj = pair.second;

            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QStringList newKeys = QStringList(keys) << it.key();
                const auto &val = it.value();
                if (val.isString()) {
                    QString newKeyStr = newKeys.join(Theme_Config_Key_Separator);
                    int idx = newKeyStr.indexOf(Theme_Config_Key_Separator);
                    if (idx <= 0 || idx == newKeyStr.size() - 1) {
                        continue;
                    }
                    QString ns = newKeyStr.left(idx);
                    namespaces.insert(ns);
                    colors[ns].insert(newKeyStr.mid(idx + 1), val.toString());
                } else if (val.isObject()) {
                    stack.push_back(qMakePair(newKeys, it.value().toObject()));
                }
            }
        }
    }

    // Get sizes
    {
        std::list<QPair<QStringList, QJsonObject>> stack;
        {
            auto it = objDoc.find(Theme_Config_Key_Sizes);
            if (it != objDoc.end() && it.value().isObject()) {
                stack.push_back(qMakePair(QStringList(), it.value().toObject()));
            }
        }

        while (!stack.empty()) {
            auto pair = stack.front();
            stack.pop_front();

            const auto &keys = pair.first;
            const auto &obj = pair.second;

            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QStringList newKeys = QStringList(keys) << it.key();
                const auto &val = it.value();
                if (val.isDouble()) {
                    QString newKeyStr = newKeys.join(Theme_Config_Key_Separator);
                    int idx = newKeyStr.indexOf(Theme_Config_Key_Separator);
                    if (idx <= 0 || idx == newKeyStr.size() - 1) {
                        continue;
                    }
                    QString ns = newKeyStr.left(idx);
                    namespaces.insert(ns);
                    sizes[ns].insert(newKeyStr.mid(idx + 1), val.toInt());
                } else if (val.isObject()) {
                    stack.push_back(qMakePair(newKeys, it.value().toObject()));
                }
            }
        }
    }

    return true;
}
