#include "QCssValueList.h"
#include "private/QMetaTypeUtils.h"

#include <QDebug>

QCssValueList QCssValueList::fromStringList(const QStringList &stringList) {
    QMETATYPE_CHECK_FUNC(stringList, strData);

    QCssValueList res;
    QStringList valueList = QMetaTypeUtils::SplitStringByComma(strData);
    for (const auto &item : valueList) {
        res.get().append(QMetaTypeUtils::StringToVariant(item.trimmed()));
    }

    return res;
}

const char *QCssValueList::metaFunctionName() {
    return "qlist";
}

QDebug operator<<(QDebug debug, const QCssValueList &list) {
    debug << list.get();
    return debug;
}