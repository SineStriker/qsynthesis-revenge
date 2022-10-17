#ifndef FILEMANAGERPRIVATE_H
#define FILEMANAGERPRIVATE_H

#include "Basic/BasicManager_p.h"

#include "FileManager.h"

class FileManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(FileManager)
public:
    FileManagerPrivate();
    ~FileManagerPrivate();

    void init();
};

#endif // FILEMANAGERPRIVATE_H
