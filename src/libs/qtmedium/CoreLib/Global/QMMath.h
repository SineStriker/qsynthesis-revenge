#ifndef QMMATH_H
#define QMMATH_H

#include <QJsonArray>
#include <QPoint>
#include <QStringList>

#include "QMGlobal.h"

namespace QMMath {

    /**
     * @brief Remove begin and end quote of a string
     *
     * @param token String
     * @return
     */
    QMCORELIB_API QString removeSideQuote(const QString &token);

    /**
     * @brief Convert a list of string to a list of int, skip if fail
     *
     * @param list String list
     * @return
     */
    QMCORELIB_API QList<int> toIntList(const QStringList &list);

    /**
     * @brief Convert a list of string to a list of double, skip if fail
     *
     * @param list String list
     * @return
     */
    QMCORELIB_API QList<double> toDoubleList(const QStringList &list);

    /**
     * @brief Iterate json array and append string element to a string list
     *
     * @param arr Json array
     * @return
     */
    QMCORELIB_API QStringList arrayToStringList(const QJsonArray &arr, bool considerNumber = false);

    /**
     * @brief Determine if a string is all of number(dot, minus)
     *
     * @param s String
     * @param considerDot Consider . as number
     * @param considerNeg Consider - as number
     */
    QMCORELIB_API bool isNumber(const QString &s, bool considerDot, bool considerNeg);

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
    QMCORELIB_API double euclideanDistance(const QPoint &p1, const QPoint &p2);

    QMCORELIB_API QStringList splitAll(const QString &str, const QChar &delim);

    /**
     * @brief Adjust file system name if there's a file with same name
     *
     */
    QMCORELIB_API QString adjustRepeatedName(const QSet<QString> &set, const QString &name);

    /**
     * @brief Move item inside the array
     *
     */
    template <template <class> class Array, class T>
    bool arrayMove(Array<T> &arr, int index, int count, int dest) {
        count = qMin(count, arr.size() - index);
        if (count <= 0 || count > arr.size() || (dest >= index && dest <= index + count)) {
            return false;
        }

        decltype(typename std::remove_reference<decltype(arr)>::type()) tmp;
        tmp.resize(count);
        std::copy(arr.begin() + index, arr.begin() + index + count, tmp.begin());

        // Do change
        int correctDest;
        if (dest > index) {
            correctDest = dest - count;
            auto sz = correctDest - index;
            for (int i = 0; i < sz; ++i) {
                arr[index + i] = arr[index + count + i];
            }
        } else {
            correctDest = dest;
            auto sz = index - dest;
            for (int i = sz - 1; i >= 0; --i) {
                arr[dest + count + i] = arr[dest + i];
            }
        }
        std::copy(tmp.begin(), tmp.end(), arr.begin() + correctDest);

        return true;
    }

    /**
     * @brief Insert sort an array
     *
     */
    template <template <class> class Array, class T>
    inline void insertSort(Array<T> &array) {
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
    template <template <class> class Array, class T>
    inline bool batchCheck(const Array<T> &list, bool fun(const T &)) {
        Array<T> res;
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (!fun(*it)) {
                return false;
            }
        }
        return true;
    }

    template <template <class> class Array, class T>
    inline Array<T> batchReplace(const Array<T> &list, T fun(const T &)) {
        Array<T> res;
        res.reserve(list.size());
        for (const auto &item : list) {
            res.push_back(func(item));
        }
        return res;
    }

    template <template <class> class Array, class T>
    inline Array<T> batchFilter(const Array<T> &list, bool fun(const T &)) {
        Array<T> res;
        res.reserve(list.size());
        for (const auto &item : list) {
            if (fun(item)) {
                res.push_back(item);
            }
        }
        return res;
    }

    template <template <class, class> class Map, class Key, class Val>
    inline Map<Key, Val> batchReplaceByKey(const Map<Key, Val> &map, Val fun(const Key &)) {
        Map<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(fun(it.key()), it.value());
        }
        return res;
    }

    template <template <class, class> class Map, class Key, class Val>
    inline Map<Key, Val> batchReplaceByVal(const Map<Key, Val> &map, Val fun(const Val &)) {
        Map<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), fun(it.value()));
        }
        return res;
    }

    template <template <class, class> class Map, class Key, class Val>
    inline Map<Key, Val> batchFilterByKey(const Map<Key, Val> &map, bool fun(const Key &)) {
        Map<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (fun(it.key())) {
                res.insert(it.key(), it.value());
            }
        }
        return res;
    }

    template <template <class, class> class Map, class Key, class Val>
    inline Map<Key, Val> batchFilterByVal(const Map<Key, Val> &map, bool fun(const Val &)) {
        Map<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (fun(it.value())) {
                res.insert(it.key(), it.value());
            }
        }
        return res;
    }

};

#endif // QMMATH_H
