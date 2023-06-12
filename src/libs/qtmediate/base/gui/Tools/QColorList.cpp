#include "QColorList.h"
#include "private/QMetaTypeUtils.h"

#include "QMCss.h"
#include "QMBatch.h"

#include <QDebug>

QStringList QColorList::toStringList() const {
    QStringList res;
    res.append(MetaFunctionName());

    QStringList colors;
    for (const auto &item : *this) {
        colors.append(item.name());
    }
    res.append(colors.join(','));

    return res;
}

QColorList QColorList::fromStringList(const QStringList &stringList) {
    QColorList res;
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (const auto &i : valueList) {
            QString str = QMBatch::removeSideQuote(i.simplified());
            res.append(QMCss::CssStringToColor(str));
        }
    }
    return res;
}

QLatin1String QColorList::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_ColorList);
}

QDebug operator<<(QDebug debug, const QColorList &list) {
    debug.noquote().nospace() << QList<QColor>(list);
    return debug;
}

uint qHash(const QColor &var, uint seed) {
    Q_UNUSED(var);
    Q_UNUSED(seed);
    return 0;
}
