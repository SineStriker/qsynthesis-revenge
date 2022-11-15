#ifndef JBFILEWATCHDIRTYCHECKER_H
#define JBFILEWATCHDIRTYCHECKER_H

#include <QMutex>
#include <QObject>

class JBLocalFileSystem;
class JBFileWatcherDirtyPaths;

class JBLocalFileSystemTimer : public QObject {
    Q_OBJECT
public:
    explicit JBLocalFileSystemTimer(JBLocalFileSystem *fs, QObject *parent = nullptr);
    ~JBLocalFileSystemTimer();

public:
    // Thread unsafe
    void start();

    // Thread unsafe
    void stop();

    bool isRunning() const;
    bool storeRefreshStatusToFiles();

private:
    void markPathsDirty(const QStringList &paths);
    void markFlatDirsDirty(const QStringList &paths);
    void markRecursiveDirsDirty(const QStringList &paths);

protected:
    JBLocalFileSystem *fs;

    QAtomicInt timerId;

    QStringList lastDirtyPaths;
    QStringList lastDirtyFlatDirs;
    QStringList lastDirtyRecursiveDirs;

    QScopedPointer<QMutex> lock;

    void timerEvent(QTimerEvent *event) override;

signals:
    void afterMarkDirtyCallback(const QStringList &paths, const QStringList &flatDirs,
                                const QStringList &recursiveDirs);
};

#endif // JBFILEWATCHDIRTYCHECKER_H
