#ifndef JBEXECUTORSERVICE_H
#define JBEXECUTORSERVICE_H

#include <QObject>
#include <QThread>

class JBExecutorService : public QObject {
    Q_OBJECT
public:
    explicit JBExecutorService(QObject *parent = nullptr);
    ~JBExecutorService();

    QThread *taskThread() const;

protected:
    QThread *m_taskThread;

signals:
};

#endif // JBEXECUTORSERVICE_H
