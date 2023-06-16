#ifndef QMBATCH_H
#define QMBATCH_H

#include <QJsonArray>
#include <QPoint>
#include <QStringList>

#include "QMGlobal.h"

namespace QMBatch {

    /**
     * @brief Remove begin and end quote of a string
     *
     * @param token String
     * @return
     */
    QMCORE_EXPORT QString removeSideQuote(const QString &token);

    /**
     * @brief Convert a list of string to a list of int, skip if fail
     *
     * @param list String list
     * @return
     */
    QMCORE_EXPORT QList<int> toIntList(const QStringList &list);

    /**
     * @brief Convert a list of string to a list of double, skip if fail
     *
     * @param list String list
     * @return
     */
    QMCORE_EXPORT QList<double> toDoubleList(const QStringList &list);

    /**
     * @brief Iterate json array and append string element to a string list
     *
     * @param arr Json array
     * @return
     */
    QMCORE_EXPORT QStringList arrayToStringList(const QJsonArray &arr, bool considerNumber = false);

    /**
     * @brief Determine if a string is all of number(dot, minus)
     *
     * @param s String
     * @param considerDot Consider . as number
     * @param considerNeg Consider - as number
     */
    QMCORE_EXPORT bool isNumber(const QString &s, bool considerDot, bool considerNeg);

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
    QMCORE_EXPORT double euclideanDistance(const QPoint &p1, const QPoint &p2);

    QMCORE_EXPORT QStringList splitAll(const QString &str, const QChar &delim);

    /**
     * @brief Adjust file system name if there's a file with same name
     *
     */
    QMCORE_EXPORT QString adjustRepeatedName(const QSet<QString> &set, const QString &name);

    /**
     * @brief Move item inside the array
     *
     */
    template <template <class> class Array, class T>
    void arrayMove(Array<T> &arr, int index, int count, int dest) {
        count = qMin(count, arr.size() - index);
        if (count <= 0 || count > arr.size() || (dest >= index && dest <= index + count)) {
            return;
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

//    /**
//     * @brief Remove a collection in a map/hash if the last item has been removed
//     *
//     */
//    template <template <class, template <class> class> class Map, template <class> class List, class K, class T>
//    bool removeCollection(Map<K, List> &map) {
//
//    }

};


#endif // QMBATCH_H
