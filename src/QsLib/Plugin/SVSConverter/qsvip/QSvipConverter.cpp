#include "QSvipConverter.h"

QSvipConverter::QSvipConverter(QObject *parent) : ISVSConverter(parent) {
}

QSvipConverter::~QSvipConverter() {
}

bool QSvipConverter::load(const QString &filename, QSvipModel *out,
                          const QMap<QString, QVariant> &args) {
    return false;
}

bool QSvipConverter::save(const QString &filename, const QSvipModel &in,
                          const QMap<QString, QVariant> &args) {
    return false;
}
