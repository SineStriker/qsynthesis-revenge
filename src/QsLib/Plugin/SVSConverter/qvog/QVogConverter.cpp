#include "QVogConverter.h"

QVogConverter::QVogConverter(QObject *parent) : ISVSConverter(parent) {
}

QVogConverter::~QVogConverter() {
}

bool QVogConverter::load(const QString &filename, QSvipModel *out,
                         const QMap<QString, QVariant> &args) {
    return false;
}

bool QVogConverter::save(const QString &filename, const QSvipModel &in,
                         const QMap<QString, QVariant> &args) {
    return false;
}
