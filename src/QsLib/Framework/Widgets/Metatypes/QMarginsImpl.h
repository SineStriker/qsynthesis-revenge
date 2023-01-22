#ifndef QMARGINSIMPL_H
#define QMARGINSIMPL_H

#include <QMargins>
#include <QVariant>

#include "QsFrameworkGlobal.h"

namespace QMarginsImpl {

    QSFRAMEWORK_API QLatin1String MetaFunctionName();

    QSFRAMEWORK_API QStringList toStringList(const QMargins &margins);

    QSFRAMEWORK_API QMargins fromStringList(const QStringList &stringList);

} // namespace QMarginsImpl

Q_DECLARE_METATYPE(QMargins)

#endif // QMARGINSIMPL_H
