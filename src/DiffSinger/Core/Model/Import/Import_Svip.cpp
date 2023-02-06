#include "../QDspxModel.h"

#include <QFile>

#include "QNrbfStream.h"

bool QDspx::fromSvip(const QString &filename, QDspxModel *out, QObject *parent) {
    QByteArray data;

    // Read file
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to load SVIP file.";
            return -1;
        }

        data = file.readAll();
        file.close();
    }

    QNrbfStream in(&data, QIODevice::ReadOnly);

    // Read name and version
    QString name;
    QString ver;
    in >> name;
    in >> ver;
    if (in.status() != QDataStream::Ok) {
        qDebug() << "Read SVIP version failed";
        return false;
    }

    // Read content
    QNrbf::XSAppModel model;
    in >> model;
    if (in.status() != QDataStream::Ok) {
        qDebug() << "Read SVIP content failed";
        return false;
    }

    qDebug() << model.trackList.size();

    return true;
}
