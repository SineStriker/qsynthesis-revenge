#include "JBFileWatcherDirtyPaths.h"

JBFileWatcherDirtyPaths::JBFileWatcherDirtyPaths() {
}

JBFileWatcherDirtyPaths::~JBFileWatcherDirtyPaths() {
}

bool JBFileWatcherDirtyPaths::isEmpty() {
    return m_dirtyPaths.isEmpty() && m_dirtyPathsRecursive.isEmpty() &&
           m_dirtyDirectories.isEmpty();
}

void JBFileWatcherDirtyPaths::clear() {
    m_dirtyPaths.clear();
    m_dirtyPathsRecursive.clear();
    m_dirtyDirectories.clear();
}

void JBFileWatcherDirtyPaths::addDirtyPath(const QString &path) {
    if (!m_dirtyPathsRecursive.contains(path)) {
        m_dirtyPaths.insert(path);
    }
}

void JBFileWatcherDirtyPaths::addDirtyPathRecursive(const QString &path) {
    m_dirtyPaths.remove(path);
    m_dirtyPathsRecursive.insert(path);
}

void JBFileWatcherDirtyPaths::addDirtyDirs(const QSet<QString> &paths) {
    m_dirtyDirectories.unite(paths);
}

QStringList JBFileWatcherDirtyPaths::dirtyPaths() const {
    return QStringList(m_dirtyPaths.begin(), m_dirtyPaths.end());
}

QStringList JBFileWatcherDirtyPaths::dirtyPathsRecursive() const {
    return QStringList(m_dirtyPathsRecursive.begin(), m_dirtyPathsRecursive.end());
}

QStringList JBFileWatcherDirtyPaths::dirtyDirectories() const {
    return QStringList(m_dirtyDirectories.begin(), m_dirtyDirectories.end());
}
