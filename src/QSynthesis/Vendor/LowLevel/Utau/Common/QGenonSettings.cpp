#include "QGenonSettings.h"

#include <QFileInfo>

using namespace Utau;

#define equalDouble(A, B) (A == B)

Genon::Genon() {
    clear();
}

Genon::Genon(const QString &filename) {
    clear();
    mFileName = filename;
}

void Genon::clear() {
    mFileName = "";
    mAlias = "";
    mOffset = 0.0;
    mConstant = 0.0;
    mBlank = 0.0;
    mPreUtterance = 0.0;
    mVoiceOverlap = 0.0;
}

bool Genon::isEmpty() const {
    return (mAlias.isEmpty() && (mOffset == 0.0) && (mOffset == 0.0) && (mConstant == 0.0) &&
            (mBlank == 0.0) && (mPreUtterance == 0.0) && (mVoiceOverlap == 0.0));
}

bool Genon::operator<(const Genon &oGenon) const {
    return mFileName.toLower() < oGenon.mFileName.toLower();
}

bool Genon::operator==(const Genon &oGenon) const {
    bool b = (mFileName == oGenon.mFileName) && (mAlias == oGenon.mAlias) &&
             equalDouble(mOffset, oGenon.mOffset) && equalDouble(mConstant, oGenon.mConstant) &&
             equalDouble(mBlank, oGenon.mBlank) &&
             equalDouble(mPreUtterance, oGenon.mPreUtterance) &&
             equalDouble(mVoiceOverlap, oGenon.mVoiceOverlap);
    return b;
}

bool Genon::operator!=(const Genon &oGenon) const {
    return !((*this) == oGenon);
}

QDebug Utau::operator<<(QDebug debug, const Genon &oGenon) {
    QString info =
        QString("QGenonSettings(file:\"%1\"; alias:\"%2\"; off:%3; con:%4; bla:%5; pre:%6; ove:%7)")
            .arg(QFileInfo(oGenon.mFileName).fileName(), oGenon.mAlias,
                 QString::number(oGenon.mOffset), QString::number(oGenon.mConstant),
                 QString::number(oGenon.mBlank), QString::number(oGenon.mPreUtterance),
                 QString::number(oGenon.mVoiceOverlap));
    debug.noquote() << info;
    return debug;
}
