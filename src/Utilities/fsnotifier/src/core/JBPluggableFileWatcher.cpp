#include "JBPluggableFileWatcher.h"
#include "JBFileWatcherNotificationSink.h"
#include "JBFileWatcherUtils.h"

#include <QDebug>
#include <QTextCodec>
#include <QTextStream>

JBPluggableFileWatcher::JBPluggableFileWatcher(QObject *parent) : QObject(parent) {
    myProcess = new QProcess(this);
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &JBPluggableFileWatcher::handleProcessFinished);
    connect(myProcess, &QProcess::errorOccurred, this, &JBPluggableFileWatcher::handleReadError);
    connect(myProcess, &QProcess::readyReadStandardOutput, this,
            &JBPluggableFileWatcher::handleReadOutput);
    connect(myProcess, &QProcess::readyReadStandardError, this,
            &JBPluggableFileWatcher::handleReadError);
}

JBPluggableFileWatcher::~JBPluggableFileWatcher() {
    if (myProcess->state() != QProcess::NotRunning) {
        killProcess();
    }
}

bool JBPluggableFileWatcher::isOperational() const {
    return myProcess->state() == QProcess::Running;
}

bool JBPluggableFileWatcher::startProcess(const QString &program, const QStringList &args) {
    myProcess->start(program, args);
    return myProcess->waitForStarted();
}

bool JBPluggableFileWatcher::killProcess() {
    myProcess->kill();
    return myProcess->waitForFinished();
}

bool JBPluggableFileWatcher::writeLine(const QString &line) {
    if (myProcess->state() != QProcess::Running) {
        return false;
    }
    jbDebug().noquote() << "[Watcher] Command:" << line;

    QByteArray bytes = line.toUtf8();
    bytes.append('\n');

    myProcess->write(bytes);
    if (!myProcess->waitForBytesWritten()) {
        return false;
    }
    return true;
}

void JBPluggableFileWatcher::notifyProcessTerminated(int exitCode,
                                                     QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)
}

void JBPluggableFileWatcher::notifyTextAvailable(const QString &line,
                                                 QProcess::ProcessChannel channel) {
    Q_UNUSED(line)
    Q_UNUSED(channel)
}

void JBPluggableFileWatcher::notifyErrorOccured(QProcess::ProcessError error) {
    Q_UNUSED(error)
}

void JBPluggableFileWatcher::notifyOnFailure(const QString &reason) {
    Q_UNUSED(reason)
}

void JBPluggableFileWatcher::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    notifyProcessTerminated(exitCode, exitStatus);
}

void JBPluggableFileWatcher::handleProcessError(QProcess::ProcessError error) {
    notifyErrorOccured(error);
}

void JBPluggableFileWatcher::handleReadOutput() {
    const QByteArray &data = myProcess->readAllStandardOutput();
    QTextStream in(data);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        notifyTextAvailable(in.readLine(), QProcess::StandardOutput);
    }
}

void JBPluggableFileWatcher::handleReadError() {
    const QByteArray &data = myProcess->readAllStandardOutput();
    QTextStream in(data);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        notifyTextAvailable(in.readLine(), QProcess::StandardError);
    }
}
