#include "UstPorter.h"

UstPorter::UstPorter(QObject *parent) : IDsPorter(parent) {
}

UstPorter::~UstPorter() {
}

IDsPorter::SupportedPorts UstPorter::ports() const {
    return Import | DragIn | ExportClip | ExportTrack;
}

IDsPorter::SupportedFormat UstPorter::format() const {
    return {tr("UTAU Sequence Texts"), "ust"};
}

bool UstPorter::load(const QString &filename, QDspxModel *out, QObject *parent) {
    return false;
}

bool UstPorter::save(const QString &filename, const QDspxModel &in, QObject *parent) {
    return false;
}
