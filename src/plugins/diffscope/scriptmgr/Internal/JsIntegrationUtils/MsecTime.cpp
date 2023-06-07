//
// Created by Crs_1 on 2023/6/2.
//

#include "MsecTime.h"
#include <QRegularExpression>
#include <QTextStream>

static inline int min2ms(int t) {
    return t * 60000;
}

static inline int sec2ms(double t) {
    return t * 1000;
}

static inline int h2ms(int t) {
    return t * 3600000;
}

namespace ScriptMgr::Internal {
    MsecTime::MsecTime(int minute, int second, int msec): MsecTime(min2ms(minute) + sec2ms(second) + msec) {
    }
    MsecTime::MsecTime(int msec) {
        Q_ASSERT(msec >= 0);
        t = msec;
    }
    MsecTime::MsecTime(const QString &str) {
        QRegularExpression rx(R"(^\s*(\d*)\s*([:\x{ff1a}]?)\s*(\d*)\s*([:\x{ff1a}]?)\s*(\d*)\s*[.\x{3002}\x{ff0e}]?\s*(\d*)\s*$)");
        auto match = rx.match(str);
        if(!match.hasMatch()) return;
        auto cap1 = match.captured(1);
        auto capColon1 = match.captured(2);
        auto cap2 = match.captured(3);
        auto capColon2 = match.captured(4);
        auto cap3 = match.captured(5);
        auto cap4 = match.captured(6);
        if(cap4.isEmpty()) {
            if(cap2.isEmpty() && cap3.isEmpty()) {
                t = sec2ms(cap1.toInt());
            } else if(cap3.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms(cap2.toInt());
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms(cap3.toInt());
            }
        } else {
            if(capColon1.isEmpty() && capColon2.isEmpty()) {
                t = sec2ms((cap1 + '.' + cap4).toDouble());
            } else if(capColon2.isEmpty()) {
                t = min2ms(cap1.toInt()) + sec2ms((cap2 + '.' + cap4).toDouble());
            } else {
                t = h2ms(cap1.toInt()) + min2ms(cap2.toInt()) + sec2ms((cap3 + '.' + cap4).toDouble());
            }
        }

    }
    int MsecTime::minute() const {
        return t / 60000;
    }
    int MsecTime::second() const {
        return t % 60000 / 1000;
    }
    int MsecTime::msec() const {
        return t % 1000;
    }
    int MsecTime::totalMsec() const {
        return t;
    }
    QString MsecTime::toString() const {
        QString str;
        QTextStream textStream(&str);
        textStream.setPadChar('0');
        textStream.setFieldWidth(2);
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream <<  minute();
        textStream.setFieldWidth(0);
        textStream << ":";
        textStream.setFieldWidth(2);
        textStream << second();
        textStream.setFieldWidth(0);
        textStream << ".";
        textStream.setFieldWidth(3);
        textStream << msec();
        textStream.flush();
        return str;
    }
} // Internal