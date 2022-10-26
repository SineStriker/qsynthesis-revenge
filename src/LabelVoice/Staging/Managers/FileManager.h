#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Basic/BasicManager.h"

#include "qsutils_macros.h"

class FileManagerPrivate;

class FileManager : public BasicManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(FileManager)
    Q_SINGLETON(FileManager)
public:
    FileManager(QObject *parent = nullptr);
    ~FileManager();

public:
    void load();
    void save();

public:
    enum RecentType { Project, Folder };
    enum ChangeType { Push, Unshift, Advance, Remove, Clear };

    void commitRecent(RecentType rType, ChangeType cType, const QString &filename = QString());
    QStringList fetchRecent(RecentType rType) const;

protected:
    FileManager(FileManagerPrivate &d, QObject *parent = nullptr);

signals:
    void recentCommited(RecentType rType);
};

#endif // FILEMANAGER_H
