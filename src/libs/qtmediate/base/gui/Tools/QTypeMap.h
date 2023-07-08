#ifndef CHORUSKIT_QTYPEMAP_H
#define CHORUSKIT_QTYPEMAP_H

#include <QMap>
#include <QVariant>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QTypeMap : public QVariantMap {
public:
    inline QTypeMap() = default;
    inline QTypeMap(std::initializer_list<std::pair<QString, QVariant>> list) {
        for (const auto &it : list) {
            insert(it.first, it.second);
        }
    }

public:
    QStringList toStringList() const;
    static QTypeMap fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QTypeMap &list);

    friend uint qHash(const QVariant &var, uint seed);
};

Q_DECLARE_METATYPE(QTypeMap)

#endif // CHORUSKIT_QTYPEMAP_H
