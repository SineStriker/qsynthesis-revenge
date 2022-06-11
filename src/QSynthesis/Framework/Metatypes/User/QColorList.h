#ifndef QCOLORLIST_H
#define QCOLORLIST_H

#include <QColor>
#include <QMetaType>

class QColorList : public QList<QColor> {
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

    friend QDebug operator<<(QDebug debug, const QColorList &list);
};

Q_DECLARE_METATYPE(QColorList)

#endif // QCOLORLIST_H
