#ifndef JBNAVIGABLEFILECONTAINERS_H
#define JBNAVIGABLEFILECONTAINERS_H

#include <QString>

#include "JBNavigableMap.h"

// STRUCT TEMPLATE less
struct JBFilePath_Comparator {
    bool operator()(const QString &path1, const QString &path2) const;
};

class JBNavigableFileSet : public JBNavigableSet<QString, JBFilePath_Comparator> {
public:
    JBNavigableFileSet &operator=(const QSet<QString> &set) {
        this->clear();
        for (auto it = set.begin(); it != set.end(); ++it) {
            this->insert(*it);
        }
        return *this;
    }
};

template <class T>
class JBNavigableFileMap : public JBNavigableMap<QString, T, JBFilePath_Comparator> {
public:
    JBNavigableFileMap &operator=(const QMap<QString, T> &map) {
        this->clear();
        for (auto it = map.begin(); it != map.end(); ++it) {
            this->insert(it.key(), it.value());
        }
        return *this;
    }
};

#endif // JBNAVIGABLEFILECONTAINERS_H
