#ifndef MATHHELPER_H
#define MATHHELPER_H

#define EXPORT_1

#include <QColor>
#include <QPoint>

namespace Math {

    EXPORT_1 QList<int> toIntList(const QStringList &list);

    EXPORT_1 QList<double> toDoubleList(const QStringList &list);

    EXPORT_1 bool isNumber(const QString &s, bool considerDot, bool considerNeg);

    EXPORT_1 inline bool isPrefixedWith(const QString &A, const QString &B) {
        return A.startsWith(B) && (A != B);
    }

    EXPORT_1 QStringList extractFunctionToStringList(const QString &str, bool *ok = nullptr);

    EXPORT_1 QColor CssStringToColor(const QString &str);

    EXPORT_1 double euclideanDistance(const QPoint &p1, const QPoint &p2);

    // Template Functions
    template <class T>
    EXPORT_1 inline void insertSort(QList<T> &array) {
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
    EXPORT_1 inline void insertSort(QVector<T> &array) {
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
    EXPORT_1 inline QList<T> batchReplace(const QList<T> &list, T fun(const T &)) {
        QList<T> res;
        for (auto it = list.begin(); it != list.end(); ++it) {
            res.append(fun(*it));
        }
        return res;
    }

    template <class T>
    EXPORT_1 inline QList<T> batchFilter(const QList<T> &list, bool fun(const T &)) {
        QList<T> res;
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (fun(*it)) {
                res.append(*it);
            }
        }
        return res;
    }

    template <class Key, class Val>
    EXPORT_1 inline QMap<Key, Val> batchReplaceByKey(const QMap<Key, Val> &map,
                                                     Val fun(const Key &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(fun(it.key()), it.value());
        }
        return res;
    }

    template <class Key, class Val>
    EXPORT_1 inline QMap<Key, Val> batchReplaceByVal(const QMap<Key, Val> &map,
                                                     Val fun(const Val &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), fun(it.value()));
        }
        return res;
    }

    template <class Key, class Val>
    EXPORT_1 inline QMap<Key, Val> batchFilterByKey(const QMap<Key, Val> &map,
                                                    bool fun(const Key &)) {
        QMap<Key, Val> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (fun(it.key())) {
                res.insert(it.key(), it.value());
            }
        }
        return res;
    }

    template <class Key, class Val>
    EXPORT_1 inline QMap<Key, Val> batchFilterByVal(const QMap<Key, Val> &map,
                                                    bool fun(const Val &)) {
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
