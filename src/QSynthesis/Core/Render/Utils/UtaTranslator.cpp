#include "UtaTranslator.h"

#include "Utau/QUstUtils.h"

static const char COMMA = ',';

QString UtaTranslator::LoadingBar(int n, int sum) {
    int i, x;
    QString s = "";

    x = n * 40 / sum;

    for (i = 1; i <= 40; ++i) {

        if (i <= x) {
            s += '#';
        } else {
            s += '-';
        }
    }

    s += '(' + QString::number(n) + '/' + QString::number(sum) + ')';

    return s;
}

QString UtaTranslator::EnvelopeUstToBat(const QString &s, const double &overlap) {
    int i;
    QString env;
    QStringList vals;
    QString strOverlap = QString::number(overlap);

    if (s.isEmpty()) {
        return ("0 5 35 0 100 100 0 " + strOverlap);
    }

    vals = s.split(COMMA);

    for (i = 0; i < vals.size(); ++i) {
        if (vals[i].isEmpty()) {
            if (i == 8) {
                vals[i] = "0"; // v4
            } else if (i == 9) {
                vals[i] = vals[1]; // p5 = p2
            } else if (i == 10) {
                vals[i] = vals[4]; // v5 = v2
            }
        }
    }

    // Complete it if the 5th point exists
    if (vals.size() == 10) {
        vals.push_back("100");
    }

    // Delete the vowel sign
    if (vals.size() >= 8) {
        vals.removeAt(7);
    }

    env = vals[0];

    for (i = 1; i < vals.size(); ++i) {
        env += " " + vals[i];

        // Overlap
        if (i == 6) {
            env += " " + strOverlap;
        }
    }

    return env;
}

QStringList UtaTranslator::EnvelopeToStringList(const QList<QUstPoint> &tpoints,
                                                const double &overlap) {
    QString strOverlap = QString::number(overlap);
    QStringList listEnv;

    if (tpoints.size() < 4) {
        listEnv << "0"
                << "5"
                << "35"
                << "0"
                << "100"
                << "100"
                << "0" << strOverlap;
        return listEnv;
    }

    listEnv << QString::number(tpoints.at(0).X);
    listEnv << QString::number(tpoints.at(1).X);
    listEnv << QString::number(tpoints.at(tpoints.size() - 2).X);
    listEnv << QString::number(tpoints.at(0).Y);
    listEnv << QString::number(tpoints.at(1).Y);
    listEnv << QString::number(tpoints.at(tpoints.size() - 2).Y);
    listEnv << QString::number(tpoints.at(tpoints.size() - 1).Y);
    listEnv << strOverlap;
    if (tpoints.size() == 5) {
        listEnv << QString::number(tpoints.at(tpoints.size() - 1).X);
        listEnv << QString::number(tpoints.at(2).X);
        listEnv << QString::number(tpoints.at(2).Y);
    } else if (tpoints.at(tpoints.size() - 1).X != 0) {
        listEnv << QString::number(tpoints.at(tpoints.size() - 1).X);
    }
    return listEnv;
}

void UtaTranslator::getCorrectPBSY(int prevNoteNum, const QString &prevLyric, int curNoteNum,
                                   QUstPoint &curPoint) {
    double y1 = (prevNoteNum <= 0) ? 0 : double((prevNoteNum - curNoteNum) * 10);
    if (!QUstUtils::isRestLyric(prevLyric)) {
        curPoint.Y = y1;
    }
}

QList<QUstPoint> UtaTranslator::getDefaultPitch(const int &prevNoteNum,
                                                    const QString &prevLyric,
                                                    const int &curNoteNum) {

    QUstPoint first(0, 0);
    QUstPoint second(0, 0);

    getCorrectPBSY(prevNoteNum, prevLyric, curNoteNum, first);
    second.P = QUstPoint::sJoin;

    QList<QUstPoint> pitch = {first, second};

    return pitch;
}

QString UtaTranslator::fixFlags(const QString &s) {
    QString s2 = "";

    for (QString::size_type i = 0; i < s.size(); ++i) {
        QChar ch = s.at(i);
        if (ch == '\"') {
            continue;
        }
        if (ch == 'e' || ch == 'E') {
            s2 += '/';
        }
        s2 += ch;
    }

    return s2;
}

QString UtaTranslator::fixFilename(const QString &filename) {
    QString s;

    for (QString::size_type i = 0; i < filename.size(); ++i) {
        switch (filename[i].toLatin1()) {
        case ' ': // Space
            s += '+';
            break;

        case '\\': // Back slash
        case '/':  // Slash
            s += '_';
            break;

        case '*': // Asterisk
            s += '$';
            break;

        case '?': // Question mark
            s += '=';
            break;

        case ':': // Colon
        case '|': // Vertical line
        case '>': // Greater than
        case '<': // Small than
            break;

        default:
            s += filename[i];
        }
    }

    return s;
}
