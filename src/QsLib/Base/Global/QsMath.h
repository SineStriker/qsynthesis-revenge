#ifndef QSMATHHELPER_H
#define QSMATHHELPER_H

#include <QPoint>
#include <QStringList>

#include "QsGlobal.h"

namespace QsMath {

    /**
     * @brief Convert a list of string to a list of int, skip if fail
     *
     * @param list String list
     * @return
     */
    QSBASE_API QList<int> toIntList(const QStringList &list);

    /**
     * @brief Convert a list of string to a list of double, skip if fail
     *
     * @param list String list
     * @return
     */
    QSBASE_API QList<double> toDoubleList(const QStringList &list);

    /**
     * @brief Determine if a string is all of number(dot, minus)
     *
     * @param s String
     * @param considerDot Consider . as number
     * @param considerNeg Consider - as number
     */
    QSBASE_API bool isNumber(const QString &s, bool considerDot, bool considerNeg);

    /**
     * @brief Determine if A starts with B and not equals to B
     *
     */
    inline bool isPrefixedWith(const QString &A, const QString &B) {
        return A.startsWith(B) && (A != B);
    }

    /**
     * @brief Calculate Euclidean distance of 2 points
     *
     */
    QSBASE_API double euclideanDistance(const QPoint &p1, const QPoint &p2);

    QSBASE_API QStringList splitAll(const QString &str, const QChar &delim);

    /**
     * @brief Adjust file system name if there's a file with same name
     *
     */
    QSBASE_API QString adjustRepeatedName(const QSet<QString> &set, const QString &name);

    /**
     * @brief Insert sort an array
     *
     */
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

    /**
     * @brief Batch functions
     *
     */
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

}; // namespace QsMath

#endif // QSMATHHELPER_H
