#ifndef QCOLORLIST_H
#define QCOLORLIST_H

#include <QColor>
#include <QMetaType>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QColorList : public QList<QColor> {
public:
    QColorList() = default;
    inline QColorList(std::initializer_list<QColor> args) : QColorList(args.begin(), args.end()) {
    }
    template <typename InputIterator, QtPrivate::IfIsInputIterator<InputIterator> = true>
    QColorList(InputIterator first, InputIterator last) : QList(first, last){};
    ~QColorList() = default;

public:
    QStringList toStringList() const;
    static QColorList fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QColorList &list);

    QList<QColor> fromSet(const QSet<QColor> &) = delete;

    friend uint qHash(const QColor &var, uint seed);
};

Q_DECLARE_METATYPE(QColorList)

#endif // QCOLORLIST_H
