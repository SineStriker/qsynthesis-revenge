#ifndef JBCANONICALPATHMAP_H
#define JBCANONICALPATHMAP_H

#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QMetaType>

#include "JBNavigableFileContainers.h"

class JBCanonicalPathMap {
public:
    JBCanonicalPathMap();
    JBCanonicalPathMap(const JBNavigableFileSet &optimizedRecursiveWatchRoots,
                       const JBNavigableFileSet &optimizedFlatWatchRoots,
                       const QList<QPair<QString, QString>> &initialPathMappings);
    ~JBCanonicalPathMap();

public:
    QPair<QStringList, QStringList> getCanonicalWatchRoots();

private:
    void initializeMappings();

public:
    void addMapping(const QList<QPair<QString, QString>> &mapping);
    bool belongsToWatchRoots(const QString &reportedPath, bool isFile) const;

    QSet<QString> mapToOriginalWatchRoots(const QString &reportedPath, bool isExact) const;
    QSet<QString> mapToUpperWatchRoots(const QString &reportedPath) const; // Custom

    QStringList applyMapping(const QString &reportedPath) const;

private:
    JBNavigableFileSet myOptimizedRecursiveWatchRoots;
    JBNavigableFileSet myOptimizedFlatWatchRoots;
    QList<QPair<QString, QString>> myInitialPathMappings;
    QMultiMap<QString, QString> myPathMappings;

    static void addPrefixedPaths(const JBNavigableFileSet &paths, const QString &prefix,
                                 JBNavigableFileSet &result);
};

Q_DECLARE_METATYPE(JBCanonicalPathMap);

#endif // JBCANONICALPATHMAP_H
