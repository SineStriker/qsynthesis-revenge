#ifndef QMARGINSIMPL_H
#define QMARGINSIMPL_H

#include <QMargins>
#include <QVariant>

namespace QMarginsImpl {

    QLatin1String MetaFunctionName();

    QStringList toStringList(const QMargins &margins);

    QMargins fromStringList(const QStringList &stringList);

} // namespace QMarginsImpl

Q_DECLARE_METATYPE(QMargins)

#endif // QMARGINSIMPL_H
