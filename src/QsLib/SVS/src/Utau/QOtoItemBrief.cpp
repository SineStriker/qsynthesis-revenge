#include "QOtoItemBrief.h"

#define equalDouble(A, B) (A == B)

QOtoItemBrief::QOtoItemBrief() {
    PreUtterance = VoiceOverlap = StartPoint = 0;
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

bool QOtoItemBrief::operator==(const QOtoItemBrief &oCorrect) const {
    return equalDouble(PreUtterance, oCorrect.PreUtterance) &&
           equalDouble(VoiceOverlap, oCorrect.VoiceOverlap) &&
           equalDouble(StartPoint, oCorrect.StartPoint);
}

bool QOtoItemBrief::operator!=(const QOtoItemBrief &oCorrect) const {
    return !((*this) == oCorrect);
}
