#include "QTypeList.h"
#include "QPixelSize.h"
#include "private/QMetaTypeUtils.h"

#include "QMCss.h"

#include <QDebug>
#include <QRegularExpression>
#include <QSize>

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
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (int i = 0; i < valueList.size(); ++i) {
            QString str = valueList.at(i).simplified();
            bool ok;
            QStringList list = QMCss::extractFunctionToStringList(str, &ok);
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
                    if (str.contains(' ')) {
                        // QSize
                        QRegularExpression regex(R"((\d+)px?\s+(\d+)px?)");
                        QRegularExpressionMatch match = regex.match(str);

                        QSize size;
                        if (match.hasMatch()) {
                            QString widthString = match.captured(1);
                            QString heightString = match.captured(2);
                            int width = widthString.toInt();
                            int height = heightString.toInt();
                            size = QSize(width, height);
                        }
                        res.append(size);
                    } else {
                        // QPixelSize
                        QVariant var;
                        var.setValue(QPixelSize::fromString(str));
                        res.append(var);
                    }
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

uint qHash(const QVariant &var, uint seed) {
    Q_UNUSED(var);
    Q_UNUSED(seed);
    return 0;
}
