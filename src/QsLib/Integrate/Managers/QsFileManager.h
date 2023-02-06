#ifndef QSFILEMANAGER_H
#define QSFILEMANAGER_H

#include "QsAbstractManager.h"

#include "QsMacros.h"

#define qsFileMgr QsFileManager::instance()

class QsFileManagerPrivate;

class QSINTEGRATE_API QsFileManager : public QsAbstractManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsFileManager)
    Q_SINGLETON(QsFileManager)
public:
    QsFileManager(QObject *parent = nullptr);
    ~QsFileManager();

public:
    bool load() override;
    bool save() override;

public:
    enum RecentType {
        Project = 0,
        // User file types

        Folder = 65536,
        // User dir types
    };
    int registerFileType(const QString &key, int hint = -1);
    int registerDirType(const QString &key, int hint = -1);

    enum ChangeType {
        Push,
        Unshift,
        Advance,
        Remove,
        Clear,
    };
    void commitRecent(int rType, int cType, const QString &filename = QString());
    QStringList fetchRecent(int rType) const;

public:
    QString openFile(const QString &title, const QString &filter, const QString &flag,
                     QWidget *parent = nullptr);
    QStringList openFiles(const QString &title, const QString &filter, const QString &flag,
                          QWidget *parent = nullptr);
    QString openDir(const QString &title, const QString &flag, QWidget *parent = nullptr);
    QString saveFile(const QString &title, const QString &filename, const QString &filter,
                     const QString &flag, QWidget *parent = nullptr);

protected:
    QsFileManager(QsFileManagerPrivate &d, QObject *parent = nullptr);

signals:
    void recentCommited(int rType);
};

#endif // QSFILEMANAGER_H
