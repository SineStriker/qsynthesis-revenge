#ifndef QSSYSTEMHELPER_H
#define QSSYSTEMHELPER_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QTextCodec>

#include "QsGlobal.h"

namespace QsFs {

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

    QSBASE_API QString PathFindFileName(const QString &path);

    QSBASE_API QString PathFindNextDir(const QString &path, const QString &dir);

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

    QSBASE_API bool combine(const QString &fileName1, const QString &fileName2,
                            const QString &newName);

    QSBASE_API void reveal(const QString &filename);

    QSBASE_API int rmPreStr(const QString &dirname, const QString &prefix);

    QSBASE_API int rmPreNum(const QString &dirname, int prefix);

    QSBASE_API QString removeTailSlashes(const QString &dirname);

    QSBASE_API QString appPath();

    QSBASE_API QString appDataPath();

    QSBASE_API QStringList FindRecursiveDirs(const QString &base, int max = -1);

    QSBASE_API QString invalidFileNameChars();

} // namespace QsFs

namespace QsOs {

    QSBASE_API void exitApp(int code);

    /**
     * @brief Display error without QtWidgets module
     *
     * @param title Title if message box is available
     * @param text Text
     */
    QSBASE_API void messageStderr(const QString &title, const QString &text);

    /**
     * @brief System Dpi base, 96 on Windows/Linux, 72 on Mac
     *
     * @return QSBASE_API
     */
    QSBASE_API int unitDpi();

    QSBASE_API bool isUserRoot();

    QSBASE_API QString fileManagerName();

    QSBASE_API QString rootUserName();

    QSBASE_API QString allFilesFilter();

} // namespace QsOs

#endif // QSSYSTEMHELPER_H
