#include "QUstxConverter.h"

#include "Ustx/UProject.h"

QUstxConverter::QUstxConverter(QObject *parent) : ISVSConverter(parent) {
}

QUstxConverter::~QUstxConverter() {
}

bool QUstxConverter::load(const QString &filename, QSvipFile *out,
                          const QMap<QString, QVariant> &args) {

    UProject ustx;
    if (!ustx.load(filename)) {
        return false;
    }
    return false;
}

bool QUstxConverter::save(const QString &filename, const QSvipFile &in,
                          const QMap<QString, QVariant> &args) {
    return false;
}
