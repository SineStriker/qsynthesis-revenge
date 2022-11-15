#include "JBFileWatcherUtils.h"

#include <QDebug>
#include <QDir>
#include <QSet>

const QStringList WatcherOpStrings = {
    "UNKNOWN", "GIVEUP", "RESET", "UNWATCHEABLE", "REMAP", "MESSAGE",
    "CREATE",  "DELETE", "STATS", "CHANGE",       "DIRTY", "RECDIRTY",
};

QString JBFileWatcherUtils::WatcherOpToString(JBFileWatcherUtils::WatcherOp op) {
    if (op < 0 || op >= WatcherOpStrings.size()) {
        jbWarning() << "[Fetal] Unexpected op index" << op;
        return WatcherOpStrings.front();
    }
    return WatcherOpStrings.at(op);
}

JBFileWatcherUtils::WatcherOp JBFileWatcherUtils::StringToWatcherOp(const QString &str) {
    int index = WatcherOpStrings.indexOf(str);
    if (index < 0) {
        return UNKNOWN;
    }
    return static_cast<WatcherOp>(index);
}

QString JBFileWatcherUtils::MessageToFailureReasonString(const QString &message) {
    return message;
}

bool JBFileWatcherUtils::SystemInfo::isWindows() {
#ifdef Q_OS_WINDOWS
    return true;
#else
    return false;
#endif
}

bool JBFileWatcherUtils::SystemInfo::isMac() {
#ifdef Q_OS_MAC
    return true;
#else
    return false;
#endif
}

bool JBFileWatcherUtils::SystemInfo::isLinux() {
#ifdef Q_OS_LINUX
    return true;
#else
    return false;
#endif
}

bool JBFileWatcherUtils::SystemInfo::isFileSystemCaseSensitive() {
    if (isLinux()) {
        return true;
    }
    return false;
}

Qt::CaseSensitivity JBFileWatcherUtils::SystemInfo::FileSystemCaseSensitivity() {
    if (isLinux()) {
        return Qt::CaseSensitive;
    }
    return Qt::CaseInsensitive;
}

int JBFileWatcherUtils::StringUtil::compare(QChar c1, QChar c2, bool ignoreCase) {
    int d = int(c1.unicode()) - int(c2.unicode());
    if (d == 0 || !ignoreCase) {
        return d;
    }
    auto u1 = c1.toUpper();
    auto u2 = c2.toUpper();
    d = int(u1.unicode()) - int(u2.unicode());
    if (d != 0) {
        d = int(u1.toLower().unicode()) - int(u2.toLower().unicode());
    }
    return d;
}

int JBFileWatcherUtils::StringUtil::lastIndexOf(const QString &s, QChar c, int start, int end) {
    start = qMax(start, 0);
    for (int i = qMin(end, s.length()) - 1; i >= start; i--) {
        if (s.at(i) == c) {
            return i;
        }
    }
    return -1;
}

int JBFileWatcherUtils::Integer::compare(int x, int y) {
    return (x < y) ? -1 : ((x == y) ? 0 : 1);
}

int JBFileWatcherUtils::PathUtil::getLastIndexOfPathSeparator(const QString &path, int end) {
    return qMax(StringUtil::lastIndexOf(path, '/', 0, end - 1),
                StringUtil::lastIndexOf(path, '\\', 0, end - 1));
}

QString JBFileWatcherUtils::PathUtil::getParentPath(const QString &path) {
    if (path.isEmpty()) {
        return "";
    }
    int end = qMax(path.lastIndexOf('/'), path.lastIndexOf('\\'));
    if (end == path.size() - 1) {
        end = getLastIndexOfPathSeparator(path, end);
    }
    if (end == -1 || end == 0) {
        return "";
    }
    if (isWindowsUNCRoot(path, end)) {
        return "";
    }
    // parent of '//host' is root
    QChar prev = path.at(end - 1);
    if (prev == '/' || prev == '\\') {
        end--;
    }
    return path.mid(0, end);
}

bool JBFileWatcherUtils::PathUtil::isWindowsUNCRoot(const QString &path,
                                                    int lastPathSeparatorPosition) {

    return SystemInfo::isWindows() && (path.startsWith("//") || path.startsWith("\\\\")) &&
           getLastIndexOfPathSeparator(path, lastPathSeparatorPosition) == 1;
}

bool JBFileWatcherUtils::FileUtil::startsWith(const QString &path, const QString &prefix) {
    return path.startsWith(prefix, SystemInfo::FileSystemCaseSensitivity());
}

bool JBFileWatcherUtils::FileUtil::pathsEqual(QString path1, QString path2) {
    if (path1 == path2) {
        return true;
    }
    if (path1.isEmpty() || path2.isEmpty()) {
        return false;
    }
    path1 = toCanonicalPath(path1);
    path2 = toCanonicalPath(path2);
    return !path1.compare(path2, SystemInfo::FileSystemCaseSensitivity());
}

QString JBFileWatcherUtils::FileUtil::toCanonicalPath(QString path) {
    return toCanonicalPath(std::move(path), QDir::separator(), true);
}

QString JBFileWatcherUtils::FileUtil::toCanonicalPath(QString path, QChar separatorChar,
                                                      bool removeLastSlash) {
    if (path.isEmpty()) {
        return path;
    }
    if (path.at(0) == '.') {
        if (path.size() == 1) {
            return "";
        }
        QChar c = path.at(1);
        if (c == '/' || c == separatorChar) {
            path = path.mid(2);
        }
    }

    if (separatorChar != '/') {
        path = path.replace(separatorChar, '/');
    }
    // trying to speedup the common case when there are no "//" or "/."
    int index = -1;
    do {
        index = path.indexOf('/', index + 1);
        QChar next = (index == path.size() - 1) ? 0 : path.at(index + 1);
        if (next == '.' || next == '/') {
            break;
        }
    } while (index != -1);
    if (index == -1) {
        if (removeLastSlash) {
            QString nullable;
            int start = processRoot(path, nullable);
            Q_UNUSED(nullable)
            int slashIndex = path.lastIndexOf('/');
            return slashIndex != -1 && slashIndex > start && slashIndex == path.size() - 1
                       ? path.mid(0, path.size() - 1)
                       : path;
        }
        return path;
    }

    QString result;
    int start = processRoot(path, result);
    int dots = 0;
    bool separator = true;

    for (int i = start; i < path.size(); ++i) {
        QChar c = path.at(i);
        if (c == '/') {
            if (!separator) {
                if (!processDots(result, dots, start)) {
                    // return resolveSymlinksAndCanonicalize(path, separatorChar, removeLastSlash);
                }
                dots = 0;
            }
            separator = true;
        } else if (c == '.') {
            if (separator || dots > 0) {
                ++dots;
            } else {
                result.append('.');
            }
            separator = false;
        } else {
            while (dots > 0) {
                result.append('.');
                dots--;
            }
            result.append(c);
            separator = false;
        }
    }

    if (dots > 0) {
        if (!processDots(result, dots, start)) {
            // return resolveSymlinksAndCanonicalize(path, separatorChar,removeLastSlash);
        }
    }

    int lastChar = result.size() - 1;
    if (removeLastSlash && lastChar >= 0 && result.at(lastChar) == '/' && lastChar > start) {
        result.remove(lastChar, 1);
    }

    return result;
}

int JBFileWatcherUtils::FileUtil::processRoot(QString &path, QString &result) {
    if (SystemInfo::isWindows() && path.size() > 1 && path.at(0) == '/' && path.at(1) == '/') {
        result.append("//");

        int hostStart = 2;
        while (hostStart < path.size() && path.at(hostStart) == '/')
            hostStart++;
        if (hostStart == path.size())
            return hostStart;
        int hostEnd = path.indexOf('/', hostStart);
        if (hostEnd < 0)
            hostEnd = path.size();
        result.append(path.midRef(hostStart, hostEnd - hostStart));
        result.append('/');

        int shareStart = hostEnd;
        while (shareStart < path.size() && path.at(shareStart) == '/')
            shareStart++;
        if (shareStart == path.size())
            return shareStart;
        int shareEnd = path.indexOf('/', shareStart);
        if (shareEnd < 0)
            shareEnd = path.size();
        result.append(path.midRef(shareStart, shareEnd - shareStart));
        result.append('/');

        return shareEnd;
    }

    if (!path.isEmpty() && path.at(0) == '/') {
        result.append('/');
        return 1;
    }

    if (path.size() > 2 && path.at(1) == ':' && path.at(2) == '/') {
        result.append(path.midRef(0, 3));
        return 3;
    }

    return 0;
}

bool JBFileWatcherUtils::FileUtil::processDots(QString &result, int dots, int start) {
    if (dots == 2) {
        int pos = -1;
        if (!result.endsWith("/../") && result != "../") {
            pos = StringUtil::lastIndexOf(result, '/', start, result.size() - 1);
            if (pos >= 0) {
                ++pos; // separator found, trim to next char
            } else if (start > 0) {
                pos = start; // path is absolute, trim to root ('/..' -> '/')
            } else if (result.length() > 0) {
                pos = 0; // path is relative, trim to default ('a/..' -> '')
            }
        }
        if (pos >= 0) {
            // if (symlinkResolver != null && symlinkResolver.isSymlink(result)) {
            //   return false;
            // }
            result.remove(pos, result.length() - pos);
        } else {
            result.append("../"); // impossible to traverse, keep as-is
        }
    } else if (dots != 1) {
        for (int i = 0; i < dots; i++) {
            result.append('.');
        }
        result.append('/');
    }
    return true;
}

bool JBFileWatcherUtils::OSAgnosticPathUtil::isSlash(QChar c) {
    return c == '/' || c == '\\';
}

bool JBFileWatcherUtils::OSAgnosticPathUtil::isUncPath(const QString &path) {
    return path.size() > 1 && isSlash(path.at(0)) && path.at(1) == path.at(0);
}

bool JBFileWatcherUtils::OSAgnosticPathUtil::startsWith(const QString &path,
                                                        const QString &prefix) {
    int pathLength = path.size(), prefixLength = prefix.size();
    if (prefixLength == 0) {
        return true;
    }
    if (prefixLength > pathLength) {
        return false;
    }

    bool ignoreCase = !SystemInfo::isFileSystemCaseSensitive();

    for (int pos = 0; pos < pathLength && pos < prefixLength; pos++) {
        QChar ch1 = path.at(pos);
        QChar ch2 = prefix.at(pos);
        if (ch1 == ch2)
            continue;
        if (ch1 == '/') {
            if (ch2 == '\\')
                continue;
            return false;
        } else if (ch1 == '\\') {
            if (ch2 == '/')
                continue;
            return false;
        } else if (ch2 == '/' || ch2 == '\\') {
            return false;
        }
        if (StringUtil::compare(ch1, ch2, ignoreCase) != 0) {
            return false;
        }
    }
    if (pathLength == prefixLength) {
        return true;
    }
    QChar lastPrefixChar = prefix.at(prefixLength - 1);
    int slashOrSeparatorIdx = prefixLength;
    if (lastPrefixChar == '/' || lastPrefixChar == '\\') {
        slashOrSeparatorIdx = prefixLength - 1;
    }
    QChar next = path.at(slashOrSeparatorIdx);
    return next == '/' || next == '\\';
}

int JBFileWatcherUtils::OSAgnosticPathUtil::COMPARATOR_compare(const QString &path1,
                                                               const QString &path2) {
    if (path1 == path2) {
        return 0;
    }

    int length1 = path1.size();
    int length2 = path2.size();
    bool ignoreCase = !SystemInfo::isFileSystemCaseSensitive();

    for (int pos = 0; pos < length1 && pos < length2; pos++) {
        QChar ch1 = path1.at(pos);
        QChar ch2 = path2.at(pos);
        if (ch1 == ch2)
            continue;
        if (ch1 == '/') {
            if (ch2 == '\\')
                continue;
            return -1;
        } else if (ch1 == '\\') {
            if (ch2 == '/')
                continue;
            return -1;
        } else if (ch2 == '/' || ch2 == '\\') {
            return 1;
        }
        int diff = StringUtil::compare(ch1, ch2, ignoreCase);
        if (diff != 0) {
            return diff;
        }
    }
    return Integer::compare(length1, length2);
}

bool JBFileWatcherUtils::WatchRootsUtil::isCoveredRecursively(
    const JBNavigableFileSet &recursiveRoots, const QString &path) {
    auto it = recursiveRoots.floor(path);
    return it != recursiveRoots.end() && OSAgnosticPathUtil::startsWith(path, *it);
}

void JBFileWatcherUtils::WatchRootsUtil::insertRecursivePath(JBNavigableFileSet &recursiveRoots,
                                                             const QString &path) {
    if (!isCoveredRecursively(recursiveRoots, path)) {
        recursiveRoots.insert(path);

        // Remove any roots covered by newly added
        JBNavigableFileSet::iterator higherIt;
        while ((higherIt = recursiveRoots.higher(path)) != recursiveRoots.end() &&
               OSAgnosticPathUtil::startsWith(*higherIt, path)) {
            recursiveRoots.remove(*higherIt);
        }
    }
}

JBNavigableFileSet
    JBFileWatcherUtils::WatchRootsUtil::optimizeFlatRoots(const QSet<QString> &flatRoots,
                                                          const JBNavigableFileSet &recursiveRoots,
                                                          bool convertToForwardSlashes) {
    JBNavigableFileSet result;
    if (convertToForwardSlashes) {
        for (QString flatRoot : flatRoots) {
            flatRoot.replace('/', '\\');
            if (!isCoveredRecursively(recursiveRoots, flatRoot)) {
                result.insert(flatRoot);
            }
        }
    } else {
        for (const QString &flatRoot : flatRoots) {
            if (!isCoveredRecursively(recursiveRoots, flatRoot)) {
                result.insert(flatRoot);
            }
        }
    }
    return result;
}
