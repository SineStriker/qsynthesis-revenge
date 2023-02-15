#include "QDspxModel.h"

#include <QFile>
#include <QJsonDocument>

using namespace QDspx;

bool Model::load(const QString &filename) {
    // Read file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray data = file.readAll();
    file.close();

    // Parse json
    QJsonParseError parseError{};
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    // Deserialize
    bool ok;
    *this = QASJsonType<QDspxModel>::fromObject(doc.object(), &ok);
    return ok;
}

bool Model::save(const QString &filename) const {
    // Create file
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    // Serialize and write
    file.write(QJsonDocument(QASJsonType<QDspxModel>::toObject(*this)).toJson());
    file.close();
    
    return true;
}
