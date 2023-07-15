#ifndef CHORUSKIT_QSVGICONEX_H
#define CHORUSKIT_QSVGICONEX_H

#include <QIcon>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QSvgIconEx : public QIcon {
public:
    QSvgIconEx() noexcept;
    QSvgIconEx(const QPixmap &pixmap);
    QSvgIconEx(const QIcon &other);
    explicit QSvgIconEx(const QString &fileName); // file or resource name
    explicit QSvgIconEx(QIconEngine *engine);

public:
    static QSvgIconEx fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QSvgIconEx &i);
};

Q_DECLARE_TYPEINFO(QSvgIconEx, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QSvgIconEx)

#endif // CHORUSKIT_QSVGICONEX_H