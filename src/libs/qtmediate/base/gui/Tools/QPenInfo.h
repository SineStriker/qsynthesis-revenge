#ifndef CHORUSKIT_QPENINFO_H
#define CHORUSKIT_QPENINFO_H

#include <QPen>
#include <QSharedData>

#include "QMGuiGlobal.h"
#include "QMNamespace.h"

class QPenInfoData;

class QMGUI_EXPORT QPenInfo : public QPen {
public:
    QPenInfo();
    QPenInfo(Qt::PenStyle style);
    QPenInfo(const QColor &color);
    QPenInfo(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine, Qt::PenCapStyle c = Qt::SquareCap,
             Qt::PenJoinStyle j = Qt::BevelJoin);
    ~QPenInfo();

    QPenInfo(const QPenInfo &other);
    QPenInfo(QPenInfo &&other) noexcept;

    QPenInfo &operator=(const QPenInfo &other);
    QPenInfo &operator=(QPenInfo &&other) noexcept;

    QPen toPen(QM::ClickState state = QM::CS_Normal) const;

    QBrush brush(QM::ClickState state = QM::CS_Normal) const;
    void setBrush(const QBrush &brush, QM::ClickState state = QM::CS_Normal);

    QColor color(QM::ClickState state = QM::CS_Normal) const;
    void setColor(const QColor &color, QM::ClickState state = QM::CS_Normal);
    void setColors(const QList<QColor> &colors);

public:
    static QPenInfo fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QPenInfo &info);

private:
    QSharedDataPointer<QPenInfoData> d;
};

Q_DECLARE_TYPEINFO(QPenInfo, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QPenInfo)

#endif // CHORUSKIT_QPENINFO_H
