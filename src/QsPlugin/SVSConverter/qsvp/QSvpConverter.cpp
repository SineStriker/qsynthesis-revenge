#include "QSvpConverter.h"

QSvpConverter::QSvpConverter(QObject *parent) : ISVSConverter(parent) {
}

QSvpConverter::~QSvpConverter() {
}

bool QSvpConverter::load(const QString &filename, QSvipFile *out) {
    return false;
}

bool QSvpConverter::save(const QString &filename, const QSvipFile &in) {
    return false;
}
