#include "QTypeMap.h"
#include "private/QMetaTypeUtils.h"

#include "QMBatch.h"

QStringList QTypeMap::toStringList() const {
    QStringList res;
    res.append(MetaFunctionName());

    auto toString = [](const QString &key, const QVariant &var) {
        QLatin1String funName = TypeToFunctionName(var.type());
        QString res;
        if (funName.isEmpty()) {
            res = key + "=" + var.toString();
        } else {
            res = key + "=" + combineFunctionStringList(var.toStringList());
        }
        return res;
    };

    QString val;
    if (!isEmpty()) {
        auto it = begin();
        val.append(toString(it.key(), it.value()));
        it++;

        for (; it != end(); ++it) {
            val.append(',');
            val.append(toString(it.key(), it.value()));
        }
    }

    res.append(val);
    return res;
}

QTypeMap QTypeMap::fromStringList(const QStringList &stringList) {
    QTypeMap res;
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (const auto &item : valueList) {
            auto eq = item.indexOf('=', 0);
            if (eq < 0)
                continue;
            res.insert(QMBatch::removeSideQuote(item.left(eq).simplified()),
                       StringToVariant(item.mid(eq + 1).simplified()));
        }
    }
    return res;
}

QLatin1String QTypeMap::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_TypeMap);
}

QDebug operator<<(QDebug debug, const QTypeMap &list) {
    debug.noquote().nospace() << QVariantMap(list);
    return debug;
}