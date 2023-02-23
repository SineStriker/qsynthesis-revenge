#ifndef QMCSS_H
#define QMCSS_H

#include <QColor>
#include <QStringList>

#include "QMWidgetsGlobal.h"

namespace QMCss {

    QMWIDGETS_API QStringList extractFunctionToStringList(const QString &str, bool *ok = nullptr);

    QMWIDGETS_API QColor CssStringToColor(const QString &str);

}

#endif // QMCSS_H
