#include "QsSystem.h"

#include "QsMacros.h"

#include <QCoreApplication>
#include <QDirIterator>
#include <QProcess>
#include <QStandardPaths>

#ifdef Q_OS_WINDOWS
#include <ShlObj.h>
#else
#include <unistd.h>
#endif

static const char Slash = '/';

QString QsFs::PathFindFileName(const QString &path) {
    QFileInfo info(path);
    if (info.isRoot()) {
        return path;
    }
    return info.fileName();
}

QString QsFs::PathFindNextDir(const QString &path, const QString &dir) {
    if (!path.startsWith(dir)) {
        return "";
    }
    QString suffix = path.mid(dir.size());
    if (suffix.startsWith(Slash)) {
        suffix = suffix.mid(1);
    }
    int slashIndex = suffix.indexOf(Slash);
    if (slashIndex < 0) {
        return suffix;
    }
    return suffix.mid(0, slashIndex);
}


bool QsFs::combine(const QString &fileName1, const QString &fileName2, const QString &newName) {
    QFile file1(fileName1);
    QFile file2(fileName2);
    QFile file3(newName);

    if (file1.open(QIODevice::ReadOnly) && file2.open(QIODevice::ReadOnly) &&
        file3.open(QIODevice::WriteOnly)) {
    } else {
        return false;
    }

    QByteArray bytes1, bytes2;
    bytes1 = file1.readAll();
    bytes2 = file2.readAll();

    file3.write(bytes1 + bytes2);

    file1.close();
    file2.close();
    file3.close();

    return true;
}

void QsFs::reveal(const QString &filename) {
    QFileInfo info(filename);
#if defined(Q_OS_WINDOWS)
    if (info.isFile()) {
        QStringList cmds;
        cmds << "/e,"
             << "/select," << QDir::toNativeSeparators(filename);
        QProcess::startDetached("explorer.exe", cmds);
    } else if (info.isDir()) {
        QStringList cmds;
        cmds << "/e,"
             << "/root," << QDir::toNativeSeparators(filename);
        QProcess::startDetached("explorer.exe", cmds);
    }
#elif defined(Q_OS_MAC)
    if (info.isDir()) {
        QString dirname = filename;
        if (!dirname.endsWith(Slash)) {
            dirname.append(Slash);
        }
        QProcess::startDetached("bash", {"-c", "open \'" + dirname + "\'"});
    } else if (info.isFile()) {
        QStringList scriptArgs;
        scriptArgs << QLatin1String("-e")
                   << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                          .arg(filename);
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
        scriptArgs.clear();
        scriptArgs << QLatin1String("-e")
                   << QLatin1String("tell application \"Finder\" to activate");
        QProcess::execute("/usr/bin/osascript", scriptArgs);
    }
#else
    if (info.isDir()) {
        QProcess::startDetached("bash", {"-c", "xdg-open \'" + filename + "\'"});
    } else if (info.isFile()) {
        QString arg = info.absolutePath();
        QProcess::startDetached("bash", {"-c", "xdg-open \'" + arg + "\'"});
    }
#endif
}

int QsFs::rmPreStr(const QString &dirname, const QString &prefix) {
    if (!isDirExist(dirname)) {
        return 0;
    }

    Q_D_EXPLORE(dirname)

    int cnt = 0;
    QFileInfo cur;
    for (auto it = d.rbegin(); it != d.rend(); ++it) {
        cur = *it;
        if (prefix.isEmpty() || cur.fileName().startsWith(prefix)) {
            QFile file(cur.filePath());
            if (file.remove()) {
                cnt++;
            }
        }
    }
    return cnt;
}

int QsFs::rmPreNum(const QString &dirname, int prefix) {
    if (!isDirExist(dirname)) {
        return 0;
    }

    Q_D_EXPLORE(dirname)

    int cnt = 0;
    QFileInfo cur;
    for (auto it = d.rbegin(); it != d.rend(); ++it) {
        cur = *it;
        QString num = QString::number(prefix);
        QString filename = cur.fileName();
        if (filename.startsWith(num) &&
            (filename.size() == num.size() || !filename.at(num.size()).isNumber())) {
            QFile file(cur.filePath());
            if (file.remove()) {
                cnt++;
            }
        }
    }

    return cnt;
}

QString QsFs::removeTailSlashes(const QString &dirname) {
    QString path = dirname;
    while (!path.isEmpty() && (path.endsWith('/') || path.endsWith('\\'))) {
        path = path.mid(0, path.size() - 1);
    }
    return path;
}

QString QsFs::appPath() {
    return qApp->applicationDirPath();
}

QString QsFs::appDataPath() {
    QString path;
#ifdef Q_OS_WINDOWS
    path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#elif defined(Q_OS_MAC)
    path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config";
#else
    path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
#endif
    QString slashName = Slash + qAppName();
    if (path.endsWith(slashName)) {
        path.chop(slashName.size());
    }
    return path;
}

QStringList QsFs::FindRecursiveDirs(const QString &base, int max) {
    QDir dir(base);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    if (!dir.exists()) {
        return {};
    }

    QDirIterator iter(dir, QDirIterator::Subdirectories);
    QStringList res;

    while (iter.hasNext()) {
        if (max >= 0 && res.size() >= max) {
            break;
        }
        iter.next();
        QFileInfo aInfo = iter.fileInfo();
        res.append(aInfo.absoluteFilePath());
    }

    return res;
}

void QsFs::exitApp(int code) {
    ::exit(code);
}

QString QsFs::invalidFileNameChars() {
    QChar ch[] = {'\"',      '<',       '>',       '|',       '\0',      (char) 1,  (char) 2,
                  (char) 3,  (char) 4,  (char) 5,  (char) 6,  (char) 7,  (char) 8,  (char) 9,
                  (char) 10, (char) 11, (char) 12, (char) 13, (char) 14, (char) 15, (char) 16,
                  (char) 17, (char) 18, (char) 19, (char) 20, (char) 21, (char) 22, (char) 23,
                  (char) 24, (char) 25, (char) 26, (char) 27, (char) 28, (char) 29, (char) 30,
                  (char) 31, ':',       '*',       '?',       '\\',      '/'};
    return QString(ch, sizeof(ch));
}

void QsOs::messageStderr(const QString &title, const QString &text) {
#ifdef Q_OS_WINDOWS
    ::MessageBoxW(0, text.toStdWString().data(), title.toStdWString().data(),
                  MB_OK | MB_TOPMOST | MB_SETFOREGROUND | MB_ICONWARNING);
#elif Q_OS_LINUX
    fputs(qPrintable(msg), stdout);
#else
    fputs(qPrintable(msg), stdout);
#endif
}

int QsOs::unitDpi() {
#ifdef Q_OS_MACOS
    return 72;
#else
    return 96;
#endif
}

bool QsOs::isUserRoot() {
    //    QString name = qgetenv("USER");
    //    if (name.isEmpty())
    //        name = qgetenv("USERNAME");

#ifdef Q_OS_WINDOWS
    return ::IsUserAnAdmin();
#else
    return geteuid() == 0;
#endif
}

QString QsOs::fileManagerName() {
#ifdef Q_OS_WINDOWS
    return QCoreApplication::tr("Explorer");
#elif defined(Q_OS_MAC)
    return QCoreApplication::tr("Finder");
#else
    return QCoreApplication::tr("File Manager");
#endif
}

QString QsOs::rootUserName() {
#if defined(Q_OS_WINDOWS)
    return QCoreApplication::tr("Administrator");
#else
    return QCoreApplication::tr("Root User");
#endif
}

QString QsOs::allFilesFilter() {
#if defined(Q_OS_WINDOWS)
    return "*.*";
#else
    return "*";
#endif
}
