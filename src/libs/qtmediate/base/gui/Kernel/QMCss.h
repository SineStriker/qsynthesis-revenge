#ifndef QMCSS_H
#define QMCSS_H

#include <QColor>
#include <QStringList>

#include "QMGuiGlobal.h"

namespace QMCss {

    QMGUI_EXPORT QStringList extractFunctionToStringList(const QString &str, bool *ok = nullptr);

    QMGUI_EXPORT QColor CssStringToColor(const QString &str);

}

#endif // QMCSS_H
