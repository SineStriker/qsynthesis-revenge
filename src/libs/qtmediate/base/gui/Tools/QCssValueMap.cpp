#include "QCssValueMap.h"
#include "private/QMetaTypeUtils.h"

#include <QDebug>

#include "QMBatch.h"

QCssValueMap QCssValueMap::fromStringList(const QStringList &stringList) {
    QMETATYPE_CHECK_FUNC(stringList, strData);

    QCssValueMap res;
    QStringList valueList = QMetaTypeUtils::SplitStringByComma(strData);

    for (auto item : qAsConst(valueList)) {
        item = item.trimmed();
        auto eq = QMetaTypeUtils::FindFirstEqualSign(item);
        if (eq < 0)
            continue;
        res.get().insert(QMBatch::removeSideQuote(item.left(eq).trimmed(), true),
                         QMetaTypeUtils::StringToVariant(item.mid(eq + 1).trimmed()));
    }
    return res;
}

const char *QCssValueMap::metaFunctionName() {
    return "qmap";
}

QDebug operator<<(QDebug debug, const QCssValueMap &map) {
    debug << map.get();
    return debug;
}