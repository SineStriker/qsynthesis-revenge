#ifndef SYSTEMHELPER_H
#define SYSTEMHELPER_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QTextCodec>

#include "qsutils_global.h"

namespace Sys {

    inline bool isPathRelative(const QString &path) {
        return QDir::isRelativePath(path);
    }

    inline bool isPathExist(const QString &path) {
        return (!isPathRelative(path) && QFileInfo::exists(path));
    }

    inline bool isFileExist(const QString &path) {
        return (!isPathRelative(path) && QFileInfo(path).isFile());
    }

    inline bool isDirExist(const QString &path) {
        return (!isPathRelative(path) && QFileInfo(path).isDir());
    }

    inline bool isSamePath(const QString &path1, const QString &path2) {
        return (QFileInfo(path1).canonicalFilePath() == QFileInfo(path2).canonicalFilePath());
    }

    inline QString PathFindSuffix(const QString &path) {
        return QFileInfo(path).suffix();
    }

    inline QString PathFildDirPath(const QString &path) {
        return QFileInfo(path).absolutePath();
    }

    QSUTILS_API QString PathFindFileName(const QString &path);

    QSUTILS_API QString PathFindNextDir(const QString &path, const QString &dir);

    inline QDateTime PathGetModifyTime(const QString &path) {
        return QFileInfo(path).lastModified().toLocalTime();
    }

    inline bool mkDir(const QString &dirname) {
        return isDirExist(dirname) || QDir(dirname).mkpath(dirname);
    }

    inline bool rmDir(const QString &dirname) {
        return isDirExist(dirname) && QDir(dirname).removeRecursively();
    }

    inline bool rmFile(const QString &filename) {
        return QFile::remove(filename);
    }

    inline bool copy(const QString &fileName, const QString &newName) {
        QFile file(newName);
        return (!file.exists() || file.remove()) && QFile::copy(fileName, newName);
    }

    QSUTILS_API bool combine(const QString &fileName1, const QString &fileName2,
                                 const QString &newName);

    QSUTILS_API void reveal(const QString &filename);

    QSUTILS_API int rmPreStr(const QString &dirname, const QString &prefix);

    QSUTILS_API int rmPreNum(const QString &dirname, int prefix);

    QSUTILS_API QString removeTailSlashes(const QString &dirname);

    QSUTILS_API QString appPath();

    QSUTILS_API QStringList FindRecursiveDirs(const QString &base, int max = -1);

    QSUTILS_API bool isUserRoot();

    QSUTILS_API void exitApp(int code);

    QSUTILS_API QString invalidFileNameChars();

} // namespace Sys

#endif // SYSTEMHELPER_H
