#include "QUtaFile.h"

QUtaFile::QUtaFile() {
}

QUtaFile::~QUtaFile() {
}

bool QUtaFile::load(const QString &filename) {
    Q_UNUSED(filename)
    return false;
}

bool QUtaFile::save(const QString &filename) {
    Q_UNUSED(filename)
    return false;
}

bool QUtaFile::isEmpty() const {
    return true;
}
