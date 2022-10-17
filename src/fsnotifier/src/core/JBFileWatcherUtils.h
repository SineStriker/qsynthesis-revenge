#ifndef JBFILEWATCHERUTILS_H
#define JBFILEWATCHERUTILS_H

#include <QDebug>
#include <QSet>
#include <QString>
#include <QStringList>

#include "JBNavigableFileContainers.h"

// #define JB_NO_DEBUG_OUTPUT
// #define JB_NO_WARNING_OUTPUT

namespace JBFileWatcherUtils {

    enum WatcherOp {
        UNKNOWN,
        GIVEUP,
        RESET,
        UNWATCHEABLE,
        REMAP,
        MESSAGE,
        CREATE,
        DELETE,
        STATS,
        CHANGE,
        DIRTY,
        RECDIRTY,
    };

    enum FailureReason {
        WATCHER_NOT_FOUND,
        NOT_EXECUTABLE,
        FAIL_TO_START,
    };

    template <class T>
    inline QSet<T> ListToSet(const QList<T> &list) {
        return QSet<T>(list.begin(), list.end());
    }

    template <class T>
    inline QList<T> SetToList(const QSet<T> &set) {
        return QList<T>(set.begin(), set.end());
    }

    QString WatcherOpToString(WatcherOp op);

    WatcherOp StringToWatcherOp(const QString &str);

    QString MessageToFailureReasonString(const QString &message);

    namespace SystemInfo {

        bool isWindows();

        bool isMac();

        bool isLinux();

        bool isFileSystemCaseSensitive();

        Qt::CaseSensitivity FileSystemCaseSensitivity();

    } // namespace SystemInfo

    namespace StringUtil {

        int compare(QChar c1, QChar c2, bool ignoreCase);

        int lastIndexOf(const QString &s, QChar c, int start, int end);

    } // namespace StringUtil

    namespace Integer {

        int compare(int x, int y);

    } // namespace Integer

    namespace PathUtil {

        int getLastIndexOfPathSeparator(const QString &path, int end);

        QString getParentPath(const QString &path);

        bool isWindowsUNCRoot(const QString &path, int lastPathSeparatorPosition);

    } // namespace PathUtil

    namespace FileUtil {

        bool startsWith(const QString &path, const QString &prefix);

        bool pathsEqual(QString path1, QString path2);

        QString toCanonicalPath(QString path);

        QString toCanonicalPath(QString path, QChar separatorChar, bool removeLastSlash);

        int processRoot(QString &path, QString &result);

        bool processDots(QString &result, int dots, int start);

    } // namespace FileUtil

    namespace OSAgnosticPathUtil {

        bool isSlash(QChar c);

        bool isUncPath(const QString &path);

        bool startsWith(const QString &path, const QString &prefix);

        int COMPARATOR_compare(const QString &path1, const QString &path2);

    } // namespace OSAgnosticPathUtil

    namespace WatchRootsUtil {

        template <class T, class F>
        void collectByPrefix(JBNavigableFileMap<T> paths, const QString &prefix, F collector) {
            typedef typename JBNavigableFileMap<T>::iterator Iterator;
            for (Iterator it = paths.higher(prefix); it != paths.end(); ++it) {
                if (OSAgnosticPathUtil::startsWith(it->first, prefix)) {
                    collector(it);
                } else {
                    break;
                }
            }
        }

        bool isCoveredRecursively(const JBNavigableFileSet &recursiveRoots, const QString &path);

        void insertRecursivePath(JBNavigableFileSet &recursiveRoots, const QString &path);

        template <class T>
        bool removeRecursivePath(JBNavigableFileSet &optimizedRecursiveRoots,
                                 const JBNavigableFileMap<T> &sourceRecursiveRoots,
                                 const QString &path) {
            if (!optimizedRecursiveRoots.contains(path)) {
                return false;
            }
            optimizedRecursiveRoots.remove(path);

            QString lastPath;
            collectByPrefix(sourceRecursiveRoots, path,
                            [&](typename JBNavigableMap<QString, T>::const_iterator it) -> void {
                                QString childPath = it->first;
                                if (lastPath.isEmpty() ||
                                    !OSAgnosticPathUtil::startsWith(childPath, lastPath)) {
                                    optimizedRecursiveRoots.insert(childPath);
                                    lastPath = childPath;
                                }
                            });

            return true;
        }

        JBNavigableFileSet optimizeFlatRoots(const QSet<QString> &flatRoots,
                                             const JBNavigableFileSet &recursiveRoots,
                                             bool convertToForwardSlashes);

        template <class F>
        void forEachPathSegment(const QString &path, QChar separator, F consumer) {
            int position = path.indexOf(separator);
            int length = path.length();
            while (position >= 0 && position < length) {
                QString subPath = path.mid(0, position);
                if (!consumer(subPath)) {
                    return;
                }
                position = path.indexOf(separator, position + 1);
            }
            consumer(path);
        }

    } // namespace WatchRootsUtil

}; // namespace JBFileWatcherUtils

#define jbDebug qDebug
#define jbWarning qDebug

#if defined(JB_NO_DEBUG_OUTPUT)
#undef jbDebug
#define jbDebug QT_NO_QDEBUG_MACRO
#endif

#if defined(JB_NO_WARNING_OUTPUT)
#undef jbWarning
#define jbWarning QT_NO_QDEBUG_MACRO
#endif

#endif // JBFILEWATCHERUTILS_H
