#include "QColorList.h"
#include "private/QMetaTypeUtils.h"

#include "QMBatch.h"
#include "QMCss.h"

#include <QDebug>

QColorList QColorList::fromStringList(const QStringList &stringList) {
    QMETATYPE_CHECK_FUNC(stringList, strData);

    QColorList res;
    QStringList valueList = QMetaTypeUtils::SplitStringByComma(strData);
    for (const auto &item : qAsConst(valueList)) {
        res.get().append(QMCss::CssStringToColor(QMBatch::removeSideQuote(item.simplified())));
    }

    return res;
}

const char *QColorList::metaFunctionName() {
    return "qcolors";
}

QDebug operator<<(QDebug debug, const QColorList &list) {
    debug << list.get();
    return debug;
}
