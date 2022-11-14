#include "QsFileManager_p.h"

#include <QStandardPaths>

#include "SystemHelper.h"

QsFileManagerPrivate::QsFileManagerPrivate() {
}

QsFileManagerPrivate::~QsFileManagerPrivate() {
}

void QsFileManagerPrivate::init() {
}

QString QsFileManagerPrivate::getLastOpenPath(const QString &type) {
    auto it = lastOpenPaths.find(type);
    if (it == lastOpenPaths.end()) {
        it = lastOpenPaths.insert(
            type, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    } else if (!Sys::isDirExist(it.value())) {
        it.value() = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    return it.value();
}

void QsFileManagerPrivate::saveLastOpenDir(const QString &type, const QString &path, bool upper) {
    lastOpenPaths.insert(type, upper ? Sys::PathFildDirPath(path) : path);
}
