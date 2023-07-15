#ifndef CHORUSKIT_QCSSVALUEMAP_H
#define CHORUSKIT_QCSSVALUEMAP_H

#include <QMap>
#include <QVariant>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QCssValueMap {
public:
    inline QCssValueMap() = default;
    inline QCssValueMap(const QVariantMap &map) : m_map(map){};
    inline QCssValueMap(std::initializer_list<std::pair<QString, QVariant>> list) {
        for (const auto &it : list) {
            m_map.insert(it.first, it.second);
        }
    }

    ~QCssValueMap() = default;

public:
    inline QVariantMap &get();
    inline const QVariantMap &get() const;

public:
    static QCssValueMap fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QCssValueMap &map);

private:
    QVariantMap m_map;
};

Q_DECLARE_TYPEINFO(QCssValueMap, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QCssValueMap)

inline QVariantMap &QCssValueMap::get() {
    return m_map;
}

inline const QVariantMap &QCssValueMap::get() const {
    return m_map;
}

#endif // CHORUSKIT_QCSSVALUEMAP_H
