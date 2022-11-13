#ifndef FILEMANAGERPRIVATE_H
#define FILEMANAGERPRIVATE_H

#include "../FileManager.h"
#include "Basic/BasicManager_p.h"
#include "Templates/QFileSet.h"

#include <QMap>

class QSINTEGRATE_API FileManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(FileManager)
public:
    FileManagerPrivate();
    ~FileManagerPrivate();

    void init();

    QFileSet projects;
    QFileSet folders;

    QMap<QString, QString> lastOpenPaths;

    QString getLastOpenPath(const QString &type);
    void saveLastOpenDir(const QString &type, const QString &path, bool upper = true);
};

#endif // FILEMANAGERPRIVATE_H
