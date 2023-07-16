#ifndef CHORUSKIT_QMSVG_H
#define CHORUSKIT_QMSVG_H

#include <QIcon>

#include "QMGuiGlobal.h"
#include "QMNamespace.h"

namespace QMSvg {

    QMGUI_EXPORT QIcon create(const QMap<QM::ClickState, QString> &fileMap,
                              const QMap<QM::ClickState, QString> &colorMap);

    QMGUI_EXPORT QIcon create(const QString &file, const QString &checkedFile = {}, const QString &color = "auto");

    QMGUI_EXPORT bool update(QIcon *icon, QM::ClickState state, const QString &salt);

    QMGUI_EXPORT QString getColor(QIcon *icon, QM::ClickState state);

    QMGUI_EXPORT void setColor(QIcon *icon, QM::ClickState state, const QString &color);
};



#endif // CHORUSKIT_QMSVG_H
