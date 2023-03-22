#include "loadconfig.h"

#include <QFile>
#include <QJsonDocument>

SplashText::SplashText() {
    rightToLeft = false;
    fontSize = 12;
}

bool LoadConfig::load(const QString &filename) {
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

    auto objDoc = doc.object();

    // Deserialize
    return qAsJsonTryGetClass(doc.object(), this);
}
