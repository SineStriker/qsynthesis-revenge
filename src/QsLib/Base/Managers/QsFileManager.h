#ifndef QSFILEMANAGER_H
#define QSFILEMANAGER_H

#include "QsAbstractManager.h"


#define qsFileMgr QsFileManager::instance()

class QsFileManagerPrivate;

class QSBASE_API QsFileManager : public QsAbstractManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsFileManager)
    Q_SINGLETON(QsFileManager)
public:
    explicit QsFileManager(QObject *parent = nullptr);
    ~QsFileManager();

protected:
    void loadImpl() override;
    void saveImpl() override;

public:
    enum FileType {
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
    void commitRecent(int fType, int cType, const QString &filename = QString());
    QStringList fetchRecent(int fType) const;

public:
    virtual QString openFile(const QString &title, const QString &filter, const QString &flag,
                             QObject *parent = nullptr);
    virtual QStringList openFiles(const QString &title, const QString &filter, const QString &flag,
                                  QObject *parent = nullptr);
    virtual QString openDir(const QString &title, const QString &flag, QObject *parent = nullptr);
    virtual QString saveFile(const QString &title, const QString &filename, const QString &filter, const QString &flag,
                             QObject *parent = nullptr);

protected:
    QsFileManager(QsFileManagerPrivate &d, QObject *parent = nullptr);

signals:
    void recentCommited(int fType);
};

#endif // QSFILEMANAGER_H
