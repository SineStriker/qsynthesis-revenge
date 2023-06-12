#ifndef QMARGINSIMPL_H
#define QMARGINSIMPL_H

#include <QMargins>
#include <QVariant>

#include "QMGuiGlobal.h"

namespace QMarginsImpl {

    QMGUI_EXPORT QLatin1String MetaFunctionName();

    QMGUI_EXPORT QStringList toStringList(const QMargins &margins);

    QMGUI_EXPORT QMargins fromStringList(const QStringList &stringList);

}

Q_DECLARE_METATYPE(QMargins)

#endif // QMARGINSIMPL_H
