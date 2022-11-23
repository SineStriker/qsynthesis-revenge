#include "QNrbfFile.h"

#include <QDataStream>
#include <QFile>

QNrbfFile::QNrbfFile() {
}

QNrbfFile::~QNrbfFile() {
}

bool QNrbfFile::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    QByteArray data = file.readAll();
    file.close();

    QDataStream in(&data, QIODevice::ReadOnly);
    while (in.atEnd()) {
    }

    return true;
}

bool QNrbfFile::save(const QString &filename) {
    return false;
}
