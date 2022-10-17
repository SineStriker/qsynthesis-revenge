#ifndef QOTOITEMBRIEF_H
#define QOTOITEMBRIEF_H

#include <QDebug>

#include "qssvs_global.h"

class QSSVS_API QOtoItemBrief {
public:
    double PreUtterance;
    double VoiceOverlap;
    double StartPoint;

    QOtoItemBrief();
    QOtoItemBrief(double pre, double ove, double stp);
    QOtoItemBrief(const std::initializer_list<double> &list);

    bool operator==(const QOtoItemBrief &oCorrect) const;
    bool operator!=(const QOtoItemBrief &oCorrect) const;

    friend QDebug operator<<(QDebug debug, const QOtoItemBrief &item) {
        QString info =
            QString("QCorrectGenon(pre:%1;ove:%2;stp:%3)")
                .arg(QString::number(item.PreUtterance), QString::number(item.VoiceOverlap),
                     QString::number(item.StartPoint));
        debug.noquote() << info;
        return debug;
    }
};

#endif // QOTOITEMBRIEF_H
