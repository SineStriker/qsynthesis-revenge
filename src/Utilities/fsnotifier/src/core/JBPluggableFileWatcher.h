#ifndef JBPLUGGABLEFILEWATCHER_H
#define JBPLUGGABLEFILEWATCHER_H

#include <QObject>
#include <QPointer>
#include <QProcess>

class JBFileWatcherNotificationSink;

/* Common process handler */

class JBPluggableFileWatcher : public QObject {
    Q_OBJECT
public:
    explicit JBPluggableFileWatcher(QObject *parent = nullptr);
    ~JBPluggableFileWatcher();

public:
    bool isOperational() const;

protected:
    QPointer<QProcess> myProcess;

    bool startProcess(const QString &program, const QStringList &args = {});
    bool killProcess();

    bool writeLine(const QString &line);

protected:
    virtual void notifyProcessTerminated(int exitCode, QProcess::ExitStatus exitStatus);
    virtual void notifyTextAvailable(const QString &line, QProcess::ProcessChannel channel);
    virtual void notifyErrorOccured(QProcess::ProcessError error);
    virtual void notifyOnFailure(const QString &reason);

private:
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleProcessError(QProcess::ProcessError error);
    void handleReadOutput();
    void handleReadError();
};

#endif // JBPLUGGABLEFILEWATCHER_H
