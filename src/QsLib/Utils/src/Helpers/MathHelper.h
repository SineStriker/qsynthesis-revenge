#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <QPoint>
#include <QStringList>

#include "qsutils_global.h"

namespace Math {

    QSUTILS_API QList<int> toIntList(const QStringList &list);

    QSUTILS_API QList<double> toDoubleList(const QStringList &list);

    QSUTILS_API bool isNumber(const QString &s, bool considerDot, bool considerNeg);

    inline bool isPrefixedWith(const QString &A, const QString &B) {
        return A.startsWith(B) && (A != B);
    }

    QSUTILS_API double euclideanDistance(const QPoint &p1, const QPoint &p2);

    QSUTILS_API QStringList splitAll(const QString &str, const QChar &delim);

    QSUTILS_API QString adjustRepeatedName(const QSet<QString> &set, const QString &name);

    // Template Functions
    template <class T>
    inline void insertSort(QList<T> &array) {
        int i, j;
        T key;

        for (j = 1; j < array.size(); ++j) {
            key = array[j];
            i = j - 1;
            while (i >= 0 && key < array[i]) {
                array[i + 1] = array[i];
                i = i - 1;
            }
            array[i + 1] = key;
        }
    }

    template <class T>
    inline void insertSort(QVector<T> &array) {
        int i, j;
        T key;

        for (j = 1; j < array.size(); ++j) {
            key = array[j];
            i = j - 1;
            while (i >= 0 && key < array[i]) {
                array[i + 1] = array[i];
                i = i - 1;
            }
            array[i + 1] = key;
        }
    }

    template <class T>
    inline bool batchCheck(const QList<T> &list, bool fun(const T &)) {
        QList<T> res;
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (!fun(*it)) {
                return false;
            }
        }
        return true;
    }

    template <class T>
    inline QList<T> batchReplace(const QList<T> &list, T fun(const T &)) {
        QList<T> res;
        for (auto it = list.begin(); it != list.end(); ++it) {
            res.append(fun(*it));
        }
        return res;
    }

    template <class T>
    inline QList<T> batchFilter(const QList<T> &list, bool fun(const T &)) {
        QList<T> res;
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (fun(*it)) {
                res.append(*it);
            }
        }
        return res;
    }

    template <class Key, class Val>
    inline QMap<Key, Val> batchReplaceByKey(const QMap<Key, Val> &map, Val fun(const Key &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(fun(it.key()), it.value());
        }
        return res;
    }

    template <class Key, class Val>
    inline QMap<Key, Val> batchReplaceByVal(const QMap<Key, Val> &map, Val fun(const Val &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), fun(it.value()));
        }
        return res;
    }

    template <class Key, class Val>
    inline QMap<Key, Val> batchFilterByKey(const QMap<Key, Val> &map, bool fun(const Key &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (fun(it.key())) {
                res.insert(it.key(), it.value());
            }
        }
        return res;
    }

    template <class Key, class Val>
    inline QMap<Key, Val> batchFilterByVal(const QMap<Key, Val> &map, bool fun(const Val &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (fun(it.value())) {
                res.insert(it.key(), it.value());
            }
        }
        return res;
    }

}; // namespace Math

#endif // MATHHELPER_H
