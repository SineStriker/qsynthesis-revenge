#include "UstPorter.h"

UstPorter::UstPorter(QObject *parent) : IDsPorter(parent) {
}

UstPorter::~UstPorter() {
}

IDsPorter::SupportedPorts UstPorter::ports() const {
    return Import | DragIn | ExportClip | ExportTrack;
}

QString UstPorter::filterString() const {
    return tr("UTAU Sequence Texts(*.ust)");
}

bool UstPorter::load(const QString &filename, QDspxModel *out,
                     const QMap<QString, QVariant> &args) {
    return false;
}

bool UstPorter::save(const QString &filename, const QDspxModel &in,
                     const QMap<QString, QVariant> &args) {
    return false;
}
