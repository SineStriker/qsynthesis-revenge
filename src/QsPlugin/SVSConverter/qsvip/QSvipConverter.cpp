#include "QSvipConverter.h"

QSvipConverter::QSvipConverter(QObject *parent) : ISVSConverter(parent) {
}

QSvipConverter::~QSvipConverter() {
}

bool QSvipConverter::load(const QString &filename, QSvipFile *out) {
    return false;
}

bool QSvipConverter::save(const QString &filename, const QSvipFile &in) {
    return false;
}
