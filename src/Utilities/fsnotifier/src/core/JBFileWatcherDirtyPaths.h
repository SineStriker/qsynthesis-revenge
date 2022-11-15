#ifndef JBFILEWATCHERDIRTYPATHS_H
#define JBFILEWATCHERDIRTYPATHS_H

#include "JBNavigableFileContainers.h"

class JBFileWatcherDirtyPaths {
public:
    JBFileWatcherDirtyPaths();
    ~JBFileWatcherDirtyPaths();

public:
    bool isEmpty();
    void clear();

    void addDirtyPath(const QString &path);
    void addDirtyPathRecursive(const QString &path);
    void addDirtyDirs(const QSet<QString> &paths);

    QStringList dirtyPaths() const;
    QStringList dirtyPathsRecursive() const;
    QStringList dirtyDirectories() const;

protected:
    JBNavigableFileSet m_dirtyPaths;
    JBNavigableFileSet m_dirtyPathsRecursive;
    JBNavigableFileSet m_dirtyDirectories;
};

#endif // JBFILEWATCHERDIRTYPATHS_H
