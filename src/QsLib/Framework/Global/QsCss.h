#ifndef QSCSSHELPER_H
#define QSCSSHELPER_H

#include <QColor>
#include <QStringList>

#include "QsFrameworkGlobal.h"

namespace QsCss {

    QSFRAMEWORK_API QStringList extractFunctionToStringList(const QString &str, bool *ok = nullptr);

    QSFRAMEWORK_API QColor CssStringToColor(const QString &str);

} // namespace QsCss

#endif // QSCSSHELPER_H