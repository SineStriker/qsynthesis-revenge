#include "QSvipModel.h"

QSvipModel::QSvipModel() {
}

QSvipModel::~QSvipModel() {
}

QJsonObject QSvipModel::toJsonObject() const {
    return {};
}

QSvipModel QSvipModel::fromJsonObject(const QJsonObject &obj) {
    return {};
}

void QSvipModel::reset() {
    Version.clear();
    SongTempoList.clear();
    TimeSignatureList.clear();
    TrackList.clear();
}
