#include "QUstConverter.h"

QUstConverter::QUstConverter(QObject *parent) : ISVSConverter(parent) {
}

QUstConverter::~QUstConverter() {
}

bool QUstConverter::load(const QString &filename, QSvipFile *out,
                         const QMap<QString, QVariant> &args) {
    return false;
}

bool QUstConverter::save(const QString &filename, const QSvipFile &in,
                         const QMap<QString, QVariant> &args) {
    return false;
}
