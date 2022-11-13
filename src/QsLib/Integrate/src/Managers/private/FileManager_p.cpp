#include "FileManager_p.h"

#include <QStandardPaths>

#include "SystemHelper.h"

FileManagerPrivate::FileManagerPrivate() {
}

FileManagerPrivate::~FileManagerPrivate() {
}

void FileManagerPrivate::init() {
}

QString FileManagerPrivate::getLastOpenPath(const QString &type) {
    auto it = lastOpenPaths.find(type);
    if (it == lastOpenPaths.end()) {
        it = lastOpenPaths.insert(
            type, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    } else if (!Sys::isDirExist(it.value())) {
        it.value() = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    return it.value();
}

void FileManagerPrivate::saveLastOpenDir(const QString &type, const QString &path, bool upper) {
    lastOpenPaths.insert(type, upper ? Sys::PathFildDirPath(path) : path);
}
