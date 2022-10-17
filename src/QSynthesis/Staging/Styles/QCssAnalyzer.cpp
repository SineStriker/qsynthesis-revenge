#include "QCssAnalyzer.h"
#include "QMetaTypeUtils.h"

#include <QDebug>
#include <QtMath>

const char FixedSizeProperty[] = "fix-";
const char MinSizeProperty[] = "min-";
const char MaxSizeProperty[] = "max-";

QCssAnalyzer::QCssAnalyzer() : m_ratio(1) {
}

QCssAnalyzer::QCssAnalyzer(const QString &filename) : m_ratio(1) {
    setFileName(filename);
}

QCssAnalyzer::~QCssAnalyzer() {
}

double QCssAnalyzer::ratio() const {
    return m_ratio;
}

void QCssAnalyzer::setRatio(double ratio) {
    m_ratio = ratio;
}

QByteArray QCssAnalyzer::readAndApply() {
    if (!isOpen()) {
        return {};
    }

    QByteArray data = readAll();

    // Fix fixed size
    {
        QByteArray res;
        QByteArray fix(FixedSizeProperty);
        QByteArray min(MinSizeProperty);
        QByteArray max(MaxSizeProperty);
        int i = 0;
        while (i < data.size()) {
            if (i >= data.size() - fix.size() + 1) {
                res.append(data.mid(i));
                break;
            }
            const QByteArray &cur = data.mid(i, fix.size());
            if (!cur.compare(fix, Qt::CaseInsensitive)) {
                int j = i + fix.size();
                int k = data.indexOf(';', j);
                int l = data.indexOf('\n', j);
                if (k >= 0 && k < l) {
                    QByteArray str = data.mid(j, k - j + 1);
                    res.append(min + str);
                    res.append(max + str);
                    i = k + 1;
                } else {
                    res.append(cur);
                    i += cur.size();
                }
            } else {
                res.append(data.at(i));
                i++;
            }
        }
        data = res;
    }

    // Fix pixel size
    if (m_ratio != 1) {
        QByteArray res;
        QByteArray px(PixelSizeUnit);
        int i = 0;
        while (i < data.size()) {
            if (i >= data.size() - px.size() + 1) {
                res.append(data.mid(i));
                break;
            }
            const QByteArray &cur = data.mid(i, px.size());
            if (!cur.compare(px, Qt::CaseInsensitive)) {
                QByteArray str;
                for (int j = res.size() - 1; j >= 0; --j) {
                    char ch = res.at(j);
                    if ((ch >= '0' && ch <= '9') || ch == '.') {
                        str.prepend(ch);
                    } else {
                        break;
                    }
                }
                if (!str.isEmpty()) {
                    int num = str.toInt();
                    double r = double(num) * m_ratio;
                    res.chop(str.size());
                    res.append(QByteArray::number(qCeil(r)));
                }
                res.append(cur);
                i += cur.size();
            } else {
                res.append(data.at(i));
                i++;
            }
        }
        data = res;
    }
    return data;
}
