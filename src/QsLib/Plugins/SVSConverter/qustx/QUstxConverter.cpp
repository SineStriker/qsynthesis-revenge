#include "QUstxConverter.h"

#include "Coders/UstxDecoder.h"
#include "Coders/UstxEncoder.h"
#include "Ustx/UProject.h"

QUstxConverter::QUstxConverter(QObject *parent) : ISVSConverter(parent) {
}

QUstxConverter::~QUstxConverter() {
}

bool QUstxConverter::load(const QString &filename, QSvipModel *out,
                          const QMap<QString, QVariant> &args) {
    Q_UNUSED(args);
    UProject ustx;
    if (!ustx.load(filename)) {
        return false;
    }
    *out = UstxDecoder().DecodeProject(ustx);
    return true;
}

bool QUstxConverter::save(const QString &filename, const QSvipModel &in,
                          const QMap<QString, QVariant> &args) {
    Q_UNUSED(args);
    return UstxEncoder().EncodeProject(in).save(filename);
}
