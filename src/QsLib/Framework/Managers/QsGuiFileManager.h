#ifndef QSGUIFILEMANAGER_H
#define QSGUIFILEMANAGER_H

#include "QsFileManager.h"

class QsGuiFileManagerPrivate;

class QsGuiFileManager : public QsFileManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsGuiFileManager)
public:
    QsGuiFileManager(QObject *parent = nullptr);
    ~QsGuiFileManager();

public:
    QString openFile(const QString &title, const QString &filter, const QString &flag,
                     QObject *parent = nullptr) override;
    QStringList openFiles(const QString &title, const QString &filter, const QString &flag,
                          QObject *parent = nullptr) override;
    QString openDir(const QString &title, const QString &flag, QObject *parent = nullptr) override;
    QString saveFile(const QString &title, const QString &filename, const QString &filter,
                     const QString &flag, QObject *parent = nullptr) override;

protected:
    QsGuiFileManager(QsGuiFileManagerPrivate &d, QObject *parent = nullptr);
};

#endif // QSGUIFILEMANAGER_H
