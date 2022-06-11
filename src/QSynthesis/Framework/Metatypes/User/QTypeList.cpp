#include "QTypeList.h"
#include "QMetaTypeUtils.h"
#include "QPixelSize.h"

#include "MathHelper.h"

#include <QDebug>

QString combineFunctionStringList(const QStringList &stringList) {
    if (stringList.size() != 2) {
        return QString();
    }
    return stringList.front() + '(' + stringList.back() + ')';
}

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
    if (stringList.size() == 2 &&
        !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (int i = 0; i < valueList.size(); ++i) {
            QString str = valueList.at(i).simplified();
            bool ok;
            QStringList list = Math::extractFunctionToStringList(str, &ok);
            if (ok) {
                int id = FunctionNameToType(list.front());
                if (id >= 0) {
                    QVariant var(list);
                    if (var.convert(id)) {
                        res.append(var);
                    } else {
                        res.append(str);
                    }
                } else {
                    res.append(str);
                }
            } else {
                if (str.endsWith(QLatin1String(PixelSizeUnit), Qt::CaseInsensitive)) {
                    QVariant var;
                    var.setValue(QPixelSize::fromString(str));
                    res.append(var);
                } else {
                    bool ok2;
                    double val = str.toDouble(&ok2);
                    if (ok2) {
                        res.append(val);
                    } else {
                        res.append(str);
                    }
                }
            }
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
