#ifndef QMSYSTEM_H
#define QMSYSTEM_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QTextCodec>

#include "QMGlobal.h"

namespace QMFs {

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

    inline QString PathFindDirPath(const QString &path) {
        return QFileInfo(path).absolutePath();
    }

    QMCORELIB_API QString PathFindFileName(const QString &path);

    QMCORELIB_API QString PathFindNextDir(const QString &path, const QString &dir);

    inline QDateTime PathGetModifyTime(const QString &path) {
        return QFileInfo(path).lastModified().toLocalTime();
    }

    inline bool mkDir(const QString &dirname) {
        return isDirExist(dirname) || QDir().mkpath(dirname);
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

    QMCORELIB_API bool combine(const QString &fileName1, const QString &fileName2, const QString &newName);

    QMCORELIB_API void reveal(const QString &filename);

    QMCORELIB_API int rmPreStr(const QString &dirname, const QString &prefix);

    QMCORELIB_API int rmPreNum(const QString &dirname, int prefix);

    QMCORELIB_API QString removeTailSlashes(const QString &dirname);

    QMCORELIB_API QString appDataPath();

    QMCORELIB_API QStringList FindRecursiveDirs(const QString &base, int max = -1);

    QMCORELIB_API QString invalidFileNameChars();

    QMCORELIB_API QString getSharedLibraryPath(void *&func);

}

namespace QMOs {

    QMCORELIB_API void exitApp(int code);

    /**
     * @brief Display error without QtWidgets module
     *
     * @param title Title if message box is available
     * @param text Text
     */
    QMCORELIB_API void messageStderr(const QString &title, const QString &text);

    /**
     * @brief System Dpi base, 96 on Windows/Linux, 72 on Mac
     *
     * @return QMCORELIB_API
     */
    QMCORELIB_API int unitDpi();

    QMCORELIB_API bool isUserRoot();

    QMCORELIB_API QString fileManagerName();

    QMCORELIB_API QString rootUserName();

    QMCORELIB_API QString allFilesFilter();

    QMCORELIB_API QString toLibFile(const QString &dir, const QString &name);

}

#endif // QMSYSTEM_H
