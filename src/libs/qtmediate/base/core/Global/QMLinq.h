#ifndef QMLINQ_H
#define QMLINQ_H

#include <QHash>
#include <QList>
#include <QMap>

namespace QMLinq {

    /**
     * @brief C# IEnumerable.Where
     *
     */
    template <template <class> class Array, class T, class Selector>
    Array<T> Where(const Array<T> &list, Selector selector) { // Same collection
        Array<T> res;
        for (const auto &item : qAsConst(list)) {
            if (selector(item)) {
                res.push_back(item);
            }
        }
        return res;
    }

    template <template <class> class Array2, template <class> class Array1, class T, class Selector>
    Array2<T> Where(const Array1<T> &list, Selector selector) { // Different collection
        Array2<T> res;
        for (const auto &item : qAsConst(list)) {
            if (selector(item)) {
                res.push_back(item);
            }
        }
        return res;
    }

    /**
     * @brief C# IEnumerable.Select
     *
     */

    template <class V, template <class> class Array, class T, class Mapper>
    Array<V> Select(const Array<T> &list, Mapper mapper) { // Same collection
        Array<V> res;
        res.reserve(list.size());
        for (const auto &item : qAsConst(list)) {
            res.push_back(mapper(item));
        }
        return res;
    }


    template <template <class> class Array2, class V, template <class> class Array1, class T, class Mapper>
    Array2<V> Select(const Array1<T> &list, Mapper mapper) { // Different collection
        Array2<V> res;
        res.reserve(list.size());
        for (const auto &item : qAsConst(list)) {
            res.push_back(mapper(item));
        }
        return res;
    }

    /**
     * @brief C# IEnumerable.Any
     *
     */
    template <template <class> class Array, class T, class Validator>
    bool Any(const Array<T> &list, Validator validator) {
        for (const auto &item : qAsConst(list)) {
            if (validator(item)) {
                return true;
            }
        }
        return false;
    }

    template <template <class> class Array, class T, class Validator>
    bool All(const Array<T> &list, Validator validator) {
        for (const auto &item : qAsConst(list)) {
            if (!validator(item)) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief C# IEnumerable.ForEach
     *
     */
    template <template <class> class Array, class T, class Func>
    void ForEach(const Array<T> &list, Func func) {
        for (const auto &item : qAsConst(list)) {
            func(item);
        }
    }

    template <class K, class T>
    QHash<K, T> MapToHash(const QMap<K, T> &map) {
        QHash<K, T> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), it.value());
        }
        return res;
    }

    template <class K, class T>
    QMap<K, T> HashToMap(const QHash<K, T> &map) {
        QMap<K, T> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), it.value());
        }
        return res;
    }

}

#endif // QMLINQ_H
