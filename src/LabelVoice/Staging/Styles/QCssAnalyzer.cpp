#include "QCssAnalyzer.h"
#include "QMetaTypeUtils.h"

#include <QDebug>
#include <QtMath>

const char FixedSizeProperty[] = "fix-";
const char MinSizeProperty[] = "min-";
const char MaxSizeProperty[] = "max-";

QCssAnalyzer::QCssAnalyzer() {
}

QCssAnalyzer::~QCssAnalyzer() {
}

QString QCssAnalyzer::apply(const QString &stylesheet, double ratio) const {
    QString data = stylesheet;

    // Fix fixed size
    {
        QString res;
        QString fix(FixedSizeProperty);
        QString min(MinSizeProperty);
        QString max(MaxSizeProperty);
        int i = 0;
        while (i < data.size()) {
            if (i >= data.size() - fix.size() + 1) {
                res.append(data.midRef(i));
                break;
            }
            const QString &cur = data.mid(i, fix.size());
            if (!cur.compare(fix, Qt::CaseInsensitive)) {
                int j = i + fix.size();
                int k = data.indexOf(';', j);
                int l = data.indexOf('\n', j);
                if (k >= 0 && k < l) {
                    QString str = data.mid(j, k - j + 1);
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
    if (ratio != 1) {
        QString res;
        QString px(PixelSizeUnit);
        int i = 0;
        while (i < data.size()) {
            if (i >= data.size() - px.size() + 1) {
                res.append(data.midRef(i));
                break;
            }
            const QString &cur = data.mid(i, px.size());
            if (!cur.compare(px, Qt::CaseInsensitive)) {
                QString str;
                for (int j = res.size() - 1; j >= 0; --j) {
                    QChar ch = res.at(j);
                    if ((ch >= '0' && ch <= '9') || ch == '.') {
                        str.prepend(ch);
                    } else {
                        break;
                    }
                }
                if (!str.isEmpty()) {
                    int num = str.toInt();
                    double r = double(num) * ratio;
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
