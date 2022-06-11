#ifndef QCORRECTGENON_H
#define QCORRECTGENON_H

#include <QDebug>

class QCorrectGenon {
public:
    double PreUtterance;
    double VoiceOverlap;
    double StartPoint;

    QCorrectGenon();
    QCorrectGenon(double pre, double ove, double stp);
    QCorrectGenon(const std::initializer_list<double> &list);

    bool operator==(const QCorrectGenon &oCorrect) const;
    bool operator!=(const QCorrectGenon &oCorrect) const;

    friend QDebug operator<<(QDebug debug, const QCorrectGenon &oCorrect);
};

#endif // QCORRECTGENON_H
