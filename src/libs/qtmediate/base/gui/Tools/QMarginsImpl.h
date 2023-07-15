#ifndef QMARGINSIMPL_H
#define QMARGINSIMPL_H

#include <QMargins>
#include <QVariant>

#include "QMGuiGlobal.h"

namespace QMarginsImpl {

    QMGUI_EXPORT QMargins fromStringList(const QStringList &stringList);

    QMGUI_EXPORT QMargins fromString(const QString &s);

    QMGUI_EXPORT const char *metaFunctionName();

}

Q_DECLARE_METATYPE(QMargins)

#endif // QMARGINSIMPL_H
