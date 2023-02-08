#include "../ProjectImport.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>

#include "QSvipModel.h"

bool Import::loadOpenSVIP(const QString &filename, QDspx::Model *out, QObject *parent) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "Not a json file.";
        return false;
    }

    bool ok;
    auto model = QSvipModel::fromJsonObject(doc.object(), &ok);
    if (!ok) {
        qDebug() << "Invalid OpenSVIP model file.";
        return false;
    }

    qDebug() << model.TrackList.size();

    return true;
}
