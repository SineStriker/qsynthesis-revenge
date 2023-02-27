#ifndef QSETTINGSJSONFORMAT_H
#define QSETTINGSJSONFORMAT_H

#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>

static bool readJsonFile(QIODevice &device, QSettings::SettingsMap &map) {
    bool ret = false;

    QJsonParseError error;
    map = QJsonDocument::fromJson(device.readAll(), &error).toVariant().toMap();

    if (error.error == QJsonParseError::NoError)
        ret = true;

    return ret;
}

static bool writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map) {
    bool ret = false;

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(QVariant::fromValue(map));
    if (device.write(jsonDocument.toJson()) != -1)
        ret = true;

    return ret;
}

#endif // QSETTINGSJSONFORMAT_H
