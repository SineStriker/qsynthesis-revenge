#ifndef FILESYSTEMNOTIFIERPRIVATE_H
#define FILESYSTEMNOTIFIERPRIVATE_H

#include <QEvent>
#include <QMap>
#include <QScopedPointer>
#include <QSet>
#include <QString>
#include <QThread>

class JBLocalFileSystem;
class FileSystemNotifier;

class FileSystemNotifierPrivate {
public:
    FileSystemNotifierPrivate(FileSystemNotifier *q);
    ~FileSystemNotifierPrivate();

    void init();

    void changeRecursivePathsToAdd(const QString &path, int n);
    void changeFlatPathsToAdd(const QString &path, int n);

    void postChange();
    void commitChange();

    bool waitForPathsSet(int msecs);

    void clearCachedPaths();

    QMap<QString, int> recursivePaths() const;
    QMap<QString, int> flatPaths() const;
    QPair<QMap<QString, int>, QMap<QString, int>> paths() const;

    FileSystemNotifier *const q; // q_ptr

    QMap<QString, int> recursivePathsToAdd;
    QMap<QString, int> flatPathsToAdd;

    JBLocalFileSystem *fs;

    bool hasChangeEvent;
    bool rootsNeedUpdate;

    bool separatorSystemDependent;

    static QStringList listPathToNativeSeparators(const QStringList &paths);
    static QStringList listPathFromNativeSeparators(const QStringList &paths);
};

#endif // FILESYSTEMNOTIFIERPRIVATE_H
