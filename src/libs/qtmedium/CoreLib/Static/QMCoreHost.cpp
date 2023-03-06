#include "QMCoreHost.h"
#include "private/QMCoreHost_p.h"

#include "QMSystem.h"

#include <QCoreApplication>
#include <QDebug>


Q_SINGLETON_DECLARE(QMCoreHost);

QMCoreHost::QMCoreHost(QObject *parent) : QMCoreHost(*new QMCoreHostPrivate(), parent) {
}

QMCoreHost::~QMCoreHost() {
}

bool QMCoreHost::isAboutToQuit() const {
    Q_D(const QMCoreHost);
    return d->isAboutToQuit;
}

QSettings::Format QMCoreHost::settingsJsonFormat() const {
    Q_D(const QMCoreHost);
    return d->settingsJsonFormat;
}

QString QMCoreHost::appDataDir() const {
    Q_D(const QMCoreHost);
    return d->appDataDir;
}

void QMCoreHost::setAppDataDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->appDataDir = dir;
}

QString QMCoreHost::tempDir() const {
    Q_D(const QMCoreHost);
    return d->tempDir;
}

void QMCoreHost::setTempDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->tempDir = dir;
}

QString QMCoreHost::libDir() const {
    Q_D(const QMCoreHost);
    return d->libDir;
}

void QMCoreHost::setLibDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->libDir = dir;
}

QString QMCoreHost::shareDir() const {
    Q_D(const QMCoreHost);
    return d->shareDir;
}

void QMCoreHost::setShareDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->shareDir = dir;
}

bool QMCoreHost::createDataAndTempDirs() const {
    auto func = [](const QString &path) {
        qDebug() << "qmcorehost:" << (QMFs::isDirExist(path) ? "find" : "create") << "directory" << path;
        if (!QMFs::mkDir(path)) {
            qmCon->MsgBox(nullptr, QMCoreConsole::Critical, qAppName(),
                          QString("Failed to create %1 directory!").arg(QMFs::PathFindFileName(path)));
            return false;
        }
        return true;
    };

    if (!func(appDataDir())) {
        return false;
    }

    if (!func(tempDir())) {
        return false;
    }

    return true;
}

QMCoreHost::QMCoreHost(QMCoreHostPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}

void QMCoreHost::_q_appAboutToQuit() {
    Q_D(QMCoreHost);
    d->isAboutToQuit = true;
}