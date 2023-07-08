#include "QTypeList.h"
#include "private/QMetaTypeUtils.h"

#include "QMCss.h"

#include <QDebug>
#include <QRegularExpression>
#include <QSize>

QStringList QTypeList::toStringList() const {
    QStringList res;
    res.append(MetaFunctionName());

    auto toString = [](const QVariant &var) {
        QLatin1String funName = TypeToFunctionName(var.type());
        QString res;
        if (funName.isEmpty()) {
            res = var.toString();
        } else {
            res = combineFunctionStringList(var.toStringList());
        }
        return res;
    };

    QString val;
    if (!isEmpty()) {
        auto it = begin();
        val.append(toString(*it));
        it++;

        for (; it != end(); ++it) {
            val.append(',');
            val.append(toString(*it));
        }
    }

    res.append(val);
    return res;
}

QTypeList QTypeList::fromStringList(const QStringList &stringList) {
    QTypeList res;
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (const auto &item : valueList) {
            res.append(StringToVariant(item));
        }
    }
    return res;
}

QLatin1String QTypeList::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_TypeList);
}

QDebug operator<<(QDebug debug, const QTypeList &list) {
    debug.noquote().nospace() << QVariantList(list);
    return debug;
}

uint qHash(const QVariant &var, uint seed) {
    return 0;
}