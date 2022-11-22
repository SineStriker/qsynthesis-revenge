#include "QSvpConverter.h"

QSvpConverter::QSvpConverter(QObject *parent) : ISVSConverter(parent) {
}

QSvpConverter::~QSvpConverter() {
}

bool QSvpConverter::load(const QString &filename, QSvipModel *out,
                         const QMap<QString, QVariant> &args) {
    return false;
}

bool QSvpConverter::save(const QString &filename, const QSvipModel &in,
                         const QMap<QString, QVariant> &args) {
    return false;
}
