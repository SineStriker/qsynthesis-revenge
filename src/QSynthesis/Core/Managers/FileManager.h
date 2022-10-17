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

protected:
    FileManager(FileManagerPrivate &d, QObject *parent = nullptr);
};

#endif // FILEMANAGER_H
