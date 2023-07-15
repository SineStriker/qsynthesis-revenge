#include "QMarginsImpl.h"
#include "private/QMetaTypeUtils.h"

#include <QDebug>

#include "QPixelSize.h"

namespace QMarginsImpl {

    QMargins fromStringList(const QStringList &stringList) {
        QMargins res;
        if (stringList.size() == 2 && !stringList.front().compare(metaFunctionName(), Qt::CaseInsensitive)) {
            auto x = QMetaTypeUtils::SplitStringToIntList(stringList.back().simplified());
            if (x.size() == 4) {
                // qmargins(left, top, right, bottom)
                res = QMargins(x.at(0), x.at(1), x.at(2), x.at(3));
            } else if (x.size() >= 2) {
                // qmargins(v, h)
                res = QMargins(x.at(1), x.at(0), x.at(1), x.at(0));
            } else if (!x.isEmpty()) {
                // qmargins(i)
                res = QMargins(x.front(), x.front(), x.front(), x.front());
            }
        }
        return res;
    }

    QMargins fromString(const QString &s) {
        QPixelSize px = QPixelSize::fromString(s);
        int value = px.value();
        return {value, value, value, value};
    }

    const char *metaFunctionName() {
        return "qmargins";
    }

}