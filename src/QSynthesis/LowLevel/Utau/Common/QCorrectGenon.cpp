#include "QCorrectGenon.h"

#define equalDouble(A, B) (A == B)

QCorrectGenon::QCorrectGenon() {
    PreUtterance = VoiceOverlap = StartPoint = 0;
}

QCorrectGenon::QCorrectGenon(double pre, double ove, double stp) {
    PreUtterance = pre;
    VoiceOverlap = ove;
    StartPoint = stp;
}

QCorrectGenon::QCorrectGenon(const std::initializer_list<double> &list) {
    Q_ASSERT(list.size() == 3);
    PreUtterance = *list.begin();
    VoiceOverlap = *std::next(list.begin());
    StartPoint = *std::next(list.begin(), 2);
}

bool QCorrectGenon::operator==(const QCorrectGenon &oCorrect) const {
    return equalDouble(PreUtterance, oCorrect.PreUtterance) &&
           equalDouble(VoiceOverlap, oCorrect.VoiceOverlap) &&
           equalDouble(StartPoint, oCorrect.StartPoint);
}

bool QCorrectGenon::operator!=(const QCorrectGenon &oCorrect) const {
    return !((*this) == oCorrect);
}

QDebug operator<<(QDebug debug, const QCorrectGenon &oCorrect) {
    QString info =
        QString("QCorrectGenon(pre:%1;ove:%2;stp:%3)")
            .arg(QString::number(oCorrect.PreUtterance), QString::number(oCorrect.VoiceOverlap),
                 QString::number(oCorrect.StartPoint));
    debug.noquote() << info;
    return debug;
}
