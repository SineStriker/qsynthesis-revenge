#ifndef QCOLORLIST_H
#define QCOLORLIST_H

#include <QColor>
#include <QMetaType>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QColorList {
public:
    QColorList() = default;
    inline QColorList(std::initializer_list<QColor> args) : m_list(args.begin(), args.end()){};
    inline QColorList(const QList<QColor> &list) : m_list(list){};

    template <class InputIterator, QtPrivate::IfIsInputIterator<InputIterator> = true>
    QColorList(InputIterator first, InputIterator last) : m_list(first, last){};

    ~QColorList() = default;

public:
    inline QList<QColor> &get();
    inline const QList<QColor> &get() const;

public:
    static QColorList fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QColorList &list);

private:
    QList<QColor> m_list;
};

Q_DECLARE_TYPEINFO(QColorList, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QColorList)

inline QList<QColor> &QColorList::get() {
    return m_list;
}

inline const QList<QColor> &QColorList::get() const {
    return m_list;
}

#endif // QCOLORLIST_H
