#include "QMarginsImpl.h"
#include "private/QMetaTypeUtils.h"

#include <QDebug>

QLatin1String QMarginsImpl::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_Margins);
}

QStringList QMarginsImpl::toStringList(const QMargins &margins) {
    return {MetaFunctionName(),
            QString::asprintf("%d%s,%d%s,%d%s,%d%s", margins.left(), PixelSizeUnit, margins.top(),
                              PixelSizeUnit, margins.right(), PixelSizeUnit, margins.bottom(),
                              PixelSizeUnit)};
}

QMargins QMarginsImpl::fromStringList(const QStringList &stringList) {
    QMargins res;
    if (stringList.size() == 2 &&
        !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QString content = stringList.back().simplified();
        QStringList valueList;
        if (content.contains(',')) {
            valueList = content.split(',');
        } else {
            valueList = content.split(' ');
        }
        QVector<int> x;
        for (int i = 0; i < valueList.size(); ++i) {
            QString str = valueList.at(i).simplified();
            QLatin1String px(PixelSizeUnit);
            if (str.endsWith(px, Qt::CaseInsensitive)) {
                str.chop(px.size());
            }
            bool isNum;
            int num = str.toInt(&isNum);
            if (isNum) {
                x.push_back(num);
            } else {
                x.push_back(0);
            }
        }
        if (x.size() == 4) {
            res = QMargins(x.at(0), x.at(1), x.at(2), x.at(3));
        } else if (x.size() >= 2) {
            res = QMargins(x.at(1), x.at(0), x.at(1), x.at(0));
        } else if (x.size() >= 1) {
            res = QMargins(x.front(), x.front(), x.front(), x.front());
        }
    }
    return res;
}
