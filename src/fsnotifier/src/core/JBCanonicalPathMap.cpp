#include "JBCanonicalPathMap.h"
#include "JBFileWatcherUtils.h"

using namespace JBFileWatcherUtils;

#include <QDir>
#include <QFileInfo>

JBCanonicalPathMap::JBCanonicalPathMap() {
    qRegisterMetaType<JBCanonicalPathMap>();
}

JBCanonicalPathMap::JBCanonicalPathMap(const JBNavigableFileSet &optimizedRecursiveWatchRoots,
                                       const JBNavigableFileSet &optimizedFlatWatchRoots,
                                       const QList<QPair<QString, QString>> &initialPathMappings) {
    myOptimizedRecursiveWatchRoots = optimizedRecursiveWatchRoots;
    myOptimizedFlatWatchRoots = optimizedFlatWatchRoots;
    myInitialPathMappings = initialPathMappings;
}

JBCanonicalPathMap::~JBCanonicalPathMap() {
}

QPair<QStringList, QStringList> JBCanonicalPathMap::getCanonicalWatchRoots() {
    initializeMappings();

    QMap<QString, QString> canonicalPathMappings;
    {
        JBNavigableFileSet unionRoots = myOptimizedRecursiveWatchRoots;
        unionRoots.unite(myOptimizedFlatWatchRoots);
        for (auto it = unionRoots.begin(); it != unionRoots.end(); ++it) {
            const QString &root = *it;
            QFileInfo info(root);
            QString canonicalRoot = info.isSymLink()
                                        ? info.symLinkTarget()
                                        : QDir::toNativeSeparators(info.canonicalFilePath());
            if (!canonicalRoot.isEmpty() &&
                OSAgnosticPathUtil::COMPARATOR_compare(canonicalRoot, root) != 0) {
                canonicalPathMappings.insert(root, canonicalRoot);
            }
        }
    }

    JBNavigableFileSet canonicalRecursiveRoots;
    for (auto it = myOptimizedRecursiveWatchRoots.begin();
         it != myOptimizedRecursiveWatchRoots.end(); ++it) {
        const QString &root = *it;
        auto canonicalIt = canonicalPathMappings.find(root);
        QString canonical;
        if (canonicalIt != canonicalPathMappings.end()) {
            canonical = canonicalIt.value();
            myPathMappings.insert(canonical, root);
        } else {
            canonical = root;
        }
        WatchRootsUtil::insertRecursivePath(canonicalRecursiveRoots, canonical);
    }

    JBNavigableFileSet canonicalFlatRoots;
    for (auto it = myOptimizedFlatWatchRoots.begin(); it != myOptimizedFlatWatchRoots.end(); ++it) {
        const QString &root = *it;
        auto canonicalIt = canonicalPathMappings.find(root);
        QString canonical;
        if (canonicalIt != canonicalPathMappings.end()) {
            canonical = canonicalIt.value();
            myPathMappings.insert(canonical, root);
        } else {
            canonical = root;
        }
        if (!WatchRootsUtil::isCoveredRecursively(canonicalRecursiveRoots, canonical)) {
            canonicalFlatRoots.insert(canonical);
        }
    }

    return qMakePair(canonicalRecursiveRoots.toQList(), canonicalFlatRoots.toQList());
}

void JBCanonicalPathMap::initializeMappings() {
    for (auto it = myInitialPathMappings.begin(); it != myInitialPathMappings.end(); ++it) {
        myPathMappings.insert(it->first, it->second);
    }
    // Free the memory
    myInitialPathMappings.clear();
}

void JBCanonicalPathMap::addMapping(const QList<QPair<QString, QString>> &mapping) {
    for (const auto &pair : mapping) {
        const QString &from = pair.first;
        const QString &to = pair.second;

        // See if we are adding a mapping that itself should be mapped to a different path
        // Example: /foo/real_path -> /foo/symlink, /foo/remapped_path -> /foo/real_path
        // In this case, if the file watcher returns /foo/remapped_path/file.txt, we want to
        // report /foo/symlink/file.txt back to IntelliJ.

        QStringList preRemapPathToWatchedPaths = applyMapping(to);
        for (auto it = preRemapPathToWatchedPaths.begin(); it != preRemapPathToWatchedPaths.end();
             ++it) {
            const auto &realWatchedPath = *it;
            myPathMappings.insert(from, realWatchedPath);
        }

        // Since there can be more than one file watcher and REMAPPING is an implementation
        // detail of the native file watcher, add the mapping as usual even if we added data
        // above.
        myPathMappings.insert(from, to);
    }
}

bool JBCanonicalPathMap::belongsToWatchRoots(const QString &reportedPath, bool isFile) const {
    return WatchRootsUtil::isCoveredRecursively(myOptimizedRecursiveWatchRoots, reportedPath) ||
           myOptimizedFlatWatchRoots.contains(reportedPath) ||
           (isFile && myOptimizedFlatWatchRoots.contains(PathUtil::getParentPath(reportedPath)));
}

QSet<QString> JBCanonicalPathMap::mapToOriginalWatchRoots(const QString &reportedPath,
                                                          bool isExact) const {
    if (myOptimizedFlatWatchRoots.isEmpty() && myOptimizedRecursiveWatchRoots.isEmpty()) {
        return {};
    }

    QStringList affectedPaths = applyMapping(reportedPath);
    JBNavigableFileSet changedPaths;

    for (auto it = affectedPaths.begin(); it != affectedPaths.end(); ++it) {
        const auto &affectedPath = *it;
        if (WatchRootsUtil::isCoveredRecursively(myOptimizedRecursiveWatchRoots, affectedPath) ||
            myOptimizedFlatWatchRoots.contains(affectedPath) ||
            (isExact &&
             myOptimizedFlatWatchRoots.contains(PathUtil::getParentPath(affectedPath)))) {
            changedPaths.insert(affectedPath);
        } else if (!isExact) {
            addPrefixedPaths(myOptimizedRecursiveWatchRoots, affectedPath, changedPaths);
            addPrefixedPaths(myOptimizedFlatWatchRoots, affectedPath, changedPaths);
        }
    }

    return changedPaths.toQSet();
}

QSet<QString> JBCanonicalPathMap::mapToUpperWatchRoots(const QString &reportedPath) const {
    if (myOptimizedFlatWatchRoots.isEmpty() && myOptimizedRecursiveWatchRoots.isEmpty()) {
        return {};
    }
    QStringList affectedPaths = applyMapping(reportedPath);
    JBNavigableFileSet changedPaths;
    for (auto it = affectedPaths.begin(); it != affectedPaths.end(); ++it) {
        const auto &affectedPath = *it;
        addPrefixedPaths(myOptimizedRecursiveWatchRoots, affectedPath, changedPaths);
        addPrefixedPaths(myOptimizedFlatWatchRoots, affectedPath, changedPaths);
    }
    return changedPaths.toQSet();
}

QStringList JBCanonicalPathMap::applyMapping(const QString &reportedPath) const {
    if (myPathMappings.isEmpty()) {
        return {reportedPath};
    }

    QStringList results(reportedPath);

    WatchRootsUtil::forEachPathSegment(
        reportedPath, QDir::separator(), [&](const QString &path) -> bool {
            QStringList mappedPaths = myPathMappings.values(path);
            for (auto it = mappedPaths.begin(); it != mappedPaths.end(); ++it) {
                const auto &mappedPath = *it;
                results.append(mappedPath + reportedPath.mid(path.length()));
            }
            return true;
        });

    return results;
}

void JBCanonicalPathMap::addPrefixedPaths(const JBNavigableFileSet &paths, const QString &prefix,
                                          JBNavigableFileSet &result) {
    auto it = paths.ceiling(prefix);
    if (it != paths.end() && FileUtil::startsWith(*it, prefix)) {
        // It's worth going for the set and iterator
        if (FileUtil::pathsEqual(*it, prefix)) {
            it++;
        }
        for (; it != paths.end(); ++it) {
            const auto &root = *it;
            if (FileUtil::startsWith(root, prefix)) {
                result.insert(root);
            } else {
                return;
            }
        }
    }
}
