#include "QSvipFile.h"

QSvipFile::QSvipFile() {
}

QSvipFile::~QSvipFile() {
}

bool QSvipFile::load(const QString &filename) {
    return false;
}

bool QSvipFile::save(const QString &filename) {
    return false;
}

void QSvipFile::reset() {
    Version.clear();
    SongTempoList.clear();
    TimeSignatureList.clear();
    TrackList.clear();
}
