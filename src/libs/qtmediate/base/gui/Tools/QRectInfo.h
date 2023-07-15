#ifndef CHORUSKIT_QRECTINFO_H
#define CHORUSKIT_QRECTINFO_H

#include <QPen>
#include <QSharedData>

#include "QMGuiGlobal.h"
#include "QMNamespace.h"

class QRectInfoData;

class QMGUI_EXPORT QRectInfo {
public:
    QRectInfo();
    ~QRectInfo();

    QRectInfo(const QRectInfo &other);
    QRectInfo(QRectInfo &&other) noexcept;

    QRectInfo &operator=(const QRectInfo &other);
    QRectInfo &operator=(QRectInfo &&other) noexcept;

public:
    QRect rect() const;
    void setRect(const QRect &rect);

    QMargins margins() const;
    void setMargins(const QMargins &margins);

    QColor color(QM::ClickState state = QM::CS_Normal) const;
    void setColor(const QColor &color, QM::ClickState state = QM::CS_Normal);
    void setColors(const QList<QColor> &colors);

    int radius() const;
    void setRadius(int r);

public:
    static QRectInfo fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QRectInfo &info);

private:
    QSharedDataPointer<QRectInfoData> d;
};

Q_DECLARE_TYPEINFO(QRectInfo, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QRectInfo)

#endif // CHORUSKIT_QRECTINFO_H
