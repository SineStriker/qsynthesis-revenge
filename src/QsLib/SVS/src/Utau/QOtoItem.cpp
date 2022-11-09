#include "QOtoItem.h"

#include <QFileInfo>

#define equalDouble(A, B) (A == B)

QOtoItem::QOtoItem() {
    QOtoItem::reset();
}

QOtoItem::QOtoItem(const QString &filename) {
    QOtoItem::reset();
    FileName = filename;
}

void QOtoItem::reset() {
    FileName = "";
    Alias = "";
    Offset = 0.0;
    Constant = 0.0;
    Blank = 0.0;
    PreUtterance = 0.0;
    VoiceOverlap = 0.0;
}

bool QOtoItem::isEmpty() const {
    return (Alias.isEmpty() && (Offset == 0.0) && (Offset == 0.0) && (Constant == 0.0) &&
            (Blank == 0.0) && (PreUtterance == 0.0) && (VoiceOverlap == 0.0));
}

bool QOtoItem::operator<(const QOtoItem &oGenon) const {
    return FileName.toLower() < oGenon.FileName.toLower();
}

bool QOtoItem::operator==(const QOtoItem &oGenon) const {
    bool b = (FileName == oGenon.FileName) && (Alias == oGenon.Alias) &&
             equalDouble(Offset, oGenon.Offset) && equalDouble(Constant, oGenon.Constant) &&
             equalDouble(Blank, oGenon.Blank) && equalDouble(PreUtterance, oGenon.PreUtterance) &&
             equalDouble(VoiceOverlap, oGenon.VoiceOverlap);
    return b;
}

bool QOtoItem::operator!=(const QOtoItem &oGenon) const {
    return !((*this) == oGenon);
}

QDebug operator<<(QDebug debug, const QOtoItem &item) {
    QString info =
        QString("QOtoItem(file:\"%1\"; alias:\"%2\"; off:%3; con:%4; bla:%5; pre:%6; ove:%7)")
            .arg(QFileInfo(item.FileName).fileName(), item.Alias, QString::number(item.Offset),
                 QString::number(item.Constant), QString::number(item.Blank),
                 QString::number(item.PreUtterance), QString::number(item.VoiceOverlap));
    debug.noquote() << info;
    return debug;
}
