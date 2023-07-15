#ifndef CHORUSKIT_QCSSVALUELIST_H
#define CHORUSKIT_QCSSVALUELIST_H

#include <QVariant>

#include <QMGuiGlobal.h>

class QMGUI_EXPORT QCssValueList {
public:
    QCssValueList() = default;
    inline QCssValueList(std::initializer_list<QVariant> args) : m_list(args.begin(), args.end()){};
    inline QCssValueList(const QVariantList &list) : m_list(list){};

    template <class InputIterator, QtPrivate::IfIsInputIterator<InputIterator> = true>
    QCssValueList(InputIterator first, InputIterator last) : m_list(first, last){};

    ~QCssValueList() = default;

public:
    inline QVariantList &get();
    inline const QVariantList &get() const;

public:
    static QCssValueList fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QCssValueList &list);

private:
    QVariantList m_list;
};

Q_DECLARE_TYPEINFO(QCssValueList, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QCssValueList)

QVariantList &QCssValueList::get() {
    return m_list;
}

const QVariantList &QCssValueList::get() const {
    return m_list;
}

#endif // CHORUSKIT_QCSSVALUELIST_H
