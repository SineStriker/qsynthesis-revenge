#include "QsGuiFileManager.h"
#include "private/QsGuiFileManager_p.h"

#include "QMSystem.h"

#include <QFileDialog>

static const char Slash = '/';

QsGuiFileManager::QsGuiFileManager(QObject *parent)
    : QsGuiFileManager(*new QsGuiFileManagerPrivate(), parent) {
}

QsGuiFileManager::~QsGuiFileManager() {
}

QString QsGuiFileManager::openFile(const QString &title, const QString &filter, const QString &flag,
                                   QObject *parent) {
    Q_D(QsGuiFileManager);
    QString path = QFileDialog::getOpenFileName(qobject_cast<QWidget *>(parent), title,
                                                d->getLastOpenPath(flag), filter);
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path);
    }
    return path;
}

QStringList QsGuiFileManager::openFiles(const QString &title, const QString &filter,
                                        const QString &flag, QObject *parent) {
    Q_D(QsGuiFileManager);
    QStringList paths = QFileDialog::getOpenFileNames(qobject_cast<QWidget *>(parent), title,
                                                      d->getLastOpenPath(flag), filter);
    if (!paths.isEmpty()) {
        d->saveLastOpenDir(flag, paths.back());
    }
    return paths;
}

QString QsGuiFileManager::openDir(const QString &title, const QString &flag, QObject *parent) {
    Q_D(QsGuiFileManager);
    QString path = QFileDialog::getExistingDirectory(qobject_cast<QWidget *>(parent), title,
                                                     d->getLastOpenPath(flag));
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path, false);
    }
    return path;
}

QString QsGuiFileManager::saveFile(const QString &title, const QString &filename,
                                   const QString &filter, const QString &flag, QObject *parent) {
    Q_D(QsGuiFileManager);
    QFileInfo info(filename);
    if (info.isRelative() || !QMFs::isDirExist(info.absolutePath())) {
        info.setFile(d->getLastOpenPath(flag) + Slash + info.fileName());
    }
    QString path = QFileDialog::getSaveFileName(qobject_cast<QWidget *>(parent), title,
                                                info.absoluteFilePath(), filter, nullptr);
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path);
    }
    return path;
}

QsGuiFileManager::QsGuiFileManager(QsGuiFileManagerPrivate &d, QObject *parent)
    : QsFileManager(d, parent) {
    d.init();
}
