#ifndef QTYPELIST_H
#define QTYPELIST_H

#include <QList>
#include <QVariant>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QTypeList : public QVariantList {
public:
    QTypeList() = default;
    inline QTypeList(std::initializer_list<QVariant> args) : QTypeList(args.begin(), args.end()) {
    }
    template <typename InputIterator, QtPrivate::IfIsInputIterator<InputIterator> = true>
    QTypeList(InputIterator first, InputIterator last) : QList(first, last){};
    ~QTypeList() = default;

public:
    QStringList toStringList() const;
    static QTypeList fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QTypeList &list);

    friend uint qHash(const QVariant &var, uint seed);
};

Q_DECLARE_METATYPE(QTypeList)

#endif // QTYPELIST_H
