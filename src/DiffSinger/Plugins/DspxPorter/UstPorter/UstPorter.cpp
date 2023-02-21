#include "UstPorter.h"

#include "QMSystem.h"

UstPorter::UstPorter(QObject *parent) : IDspxPorter(parent) {
    dd.setDir(QMFs::PathFindDirPath(path));
    dd.loadDefault("UstPorter");
}

UstPorter::~UstPorter() {
}

IDspxPorter::SupportedPorts UstPorter::ports() const {
    return Import | DragIn | ExportClip | ExportTrack;
}

IDspxPorter::SupportedFormat UstPorter::format() const {
    return {tr("UTAU Sequence Texts"), {"ust"}};
}

bool UstPorter::load(const QString &filename, QDspxModel *out, QObject *parent) {
    return false;
}

bool UstPorter::save(const QString &filename, const QDspxModel &in, QObject *parent) {
    return false;
}
