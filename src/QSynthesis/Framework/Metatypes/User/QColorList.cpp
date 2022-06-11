#include "QColorList.h"
#include "MathHelper.h"
#include "QMetaTypeUtils.h"

#include <QDebug>

QStringList QColorList::toStringList() const {
    QStringList res;
    res.append(MetaFunctionName());

    QString val;
    if (!isEmpty()) {
        auto it = begin();
        val.append(it->name());
        it++;

        for (; it != end(); ++it) {
            val.append(',');
            val.append(it->name());
        }
    }

    res.append(val);
    return res;
}

QColorList QColorList::fromStringList(const QStringList &stringList) {
    QColorList res;
    if (stringList.size() == 2 &&
        !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (int i = 0; i < valueList.size(); ++i) {
            QString str = valueList.at(i).simplified();
            res.append(Math::CssStringToColor(str));
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
