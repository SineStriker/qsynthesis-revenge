#include "QOtoItemBrief.h"

#define equalDouble(A, B) (A == B)

QOtoItemBrief::QOtoItemBrief() {
    QOtoItemBrief::reset();
}

QOtoItemBrief::QOtoItemBrief(double pre, double ove, double stp) {
    PreUtterance = pre;
    VoiceOverlap = ove;
    StartPoint = stp;
}

QOtoItemBrief::QOtoItemBrief(const std::initializer_list<double> &list) {
    Q_ASSERT(list.size() == 3);
    PreUtterance = *list.begin();
    VoiceOverlap = *std::next(list.begin());
    StartPoint = *std::next(list.begin(), 2);
}

void QOtoItemBrief::reset() {
    PreUtterance = VoiceOverlap = StartPoint = 0;
}

bool QOtoItemBrief::isEmpty() const {
    return (PreUtterance == 0) && (VoiceOverlap == 0) && (StartPoint == 0);
}

bool QOtoItemBrief::operator==(const QOtoItemBrief &oCorrect) const {
    return equalDouble(PreUtterance, oCorrect.PreUtterance) && equalDouble(VoiceOverlap, oCorrect.VoiceOverlap) &&
           equalDouble(StartPoint, oCorrect.StartPoint);
}

bool QOtoItemBrief::operator!=(const QOtoItemBrief &oCorrect) const {
    return !((*this) == oCorrect);
}

QDebug operator<<(QDebug debug, const QOtoItemBrief &item) {
    QString info = QString("QOtoItemBrief(pre:%1;ove:%2;stp:%3)")
                       .arg(QString::number(item.PreUtterance), QString::number(item.VoiceOverlap),
                            QString::number(item.StartPoint));
    debug.noquote() << info;
    return debug;
}