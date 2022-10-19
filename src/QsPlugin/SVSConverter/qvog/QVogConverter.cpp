#include "QVogConverter.h"

QVogConverter::QVogConverter(QObject *parent) : ISVSConverter(parent) {
}

QVogConverter::~QVogConverter() {
}

bool QVogConverter::load(const QString &filename, QSvipFile *out) {
    return false;
}

bool QVogConverter::save(const QString &filename, const QSvipFile &in) {
    return false;
}
