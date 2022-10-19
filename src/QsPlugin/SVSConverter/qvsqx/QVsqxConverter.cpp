#include "QVsqxConverter.h"

QVsqxConverter::QVsqxConverter(QObject *parent) : ISVSConverter(parent) {
}

QVsqxConverter::~QVsqxConverter() {
}

bool QVsqxConverter::load(const QString &filename, QSvipFile *out) {
    return false;
}

bool QVsqxConverter::save(const QString &filename, const QSvipFile &in) {
    return false;
}
