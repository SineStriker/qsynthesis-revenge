#include "QUstConverter.h"

QUstConverter::QUstConverter(QObject *parent) : ISVSConverter(parent) {
}

QUstConverter::~QUstConverter() {
}

bool QUstConverter::load(const QString &filename, QSvipFile *out) {
    return false;
}

bool QUstConverter::save(const QString &filename, const QSvipFile &in) {
    return false;
}
