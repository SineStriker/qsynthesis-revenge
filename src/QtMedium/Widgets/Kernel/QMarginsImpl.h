#ifndef QMARGINSIMPL_H
#define QMARGINSIMPL_H

#include <QMargins>
#include <QVariant>

#include "QMWidgetsGlobal.h"

namespace QMarginsImpl {

    QMWIDGETS_API QLatin1String MetaFunctionName();

    QMWIDGETS_API QStringList toStringList(const QMargins &margins);

    QMWIDGETS_API QMargins fromStringList(const QStringList &stringList);

}

Q_DECLARE_METATYPE(QMargins)

#endif // QMARGINSIMPL_H
