#ifndef CHORUSKIT_QFONTINFOEX_H
#define CHORUSKIT_QFONTINFOEX_H

#include <QFont>
#include <QSharedData>

#include "QMGuiGlobal.h"
#include "QMNamespace.h"

class QFontInfoExData;

class QMGUI_EXPORT QFontInfoEx {
public:
    QFontInfoEx();
    explicit QFontInfoEx(const QString &family, int pixelSize = -1, int weight = -1, bool italic = false);
    ~QFontInfoEx();

    QFontInfoEx(const QFontInfoEx &other);
    QFontInfoEx(QFontInfoEx &&other) noexcept;

    QFont toFont(const QObject *obj = nullptr) const;

    QFontInfoEx &operator=(const QFontInfoEx &other);
    QFontInfoEx &operator=(QFontInfoEx &&other) noexcept;

public:
    int weight() const;
    void setWeight(int weight);

    bool italic() const;
    void setItalic(bool italic);

    double pointSize() const;
    void setPointSize(double pointSize);

    int pixelSize() const;
    void setPixelSize(int pixelSize);

    QStringList families() const;
    void setFamilies(const QStringList &families);

    QColor color(QM::ClickState state = QM::CS_Normal) const;
    void setColor(const QColor &color, QM::ClickState state = QM::CS_Normal);
    void setColors(const QList<QColor> &colors);

public:
    static QFontInfoEx fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QFontInfoEx &info);

private:
    QSharedDataPointer<QFontInfoExData> d;
};

Q_DECLARE_TYPEINFO(QFontInfoEx, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QFontInfoEx)

#endif // CHORUSKIT_QFONTINFOEX_H
