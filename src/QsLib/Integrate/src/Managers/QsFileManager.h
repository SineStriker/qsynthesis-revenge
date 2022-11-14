#ifndef QSFILEMANAGER_H
#define QSFILEMANAGER_H

#include "QsAbstractManager.h"

#include "qsintegrate_global.h"
#include "qsutils_macros.h"

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
    enum RecentType { Project, Folder };
    enum ChangeType { Push, Unshift, Advance, Remove, Clear };

    void commitRecent(RecentType rType, ChangeType cType, const QString &filename = QString());
    QStringList fetchRecent(RecentType rType) const;

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
    void recentCommited(RecentType rType);
};

#endif // QSFILEMANAGER_H
