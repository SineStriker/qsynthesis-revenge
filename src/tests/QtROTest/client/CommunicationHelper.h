//
// Created by Crs_1 on 2023/5/16.
//

#ifndef CHORUSKIT_COMMUNICATIONHELPER_H
#define CHORUSKIT_COMMUNICATIONHELPER_H

#include <QtRemoteObjects>

Q_DECLARE_METATYPE(std::function<QVariant()>)
Q_DECLARE_METATYPE(std::function<void()>)

class Awaiter: public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void call(std::function<QVariant()> fx);
    Q_INVOKABLE void callBySignal(std::function<void()> fx);
public slots:
    void finished(const QVariant &val);
private:
    friend class CommunicationHelper;
    bool m_isFinished = false;
    QVariant m_ret;
    QMutex m_mutex;
    QWaitCondition m_condition;
};

class CommunicationHelper {
public:
    explicit CommunicationHelper();
    ~CommunicationHelper();
    void start();
    template <typename T>
    T invokeSync(const std::function<T()>& fx) {
        Awaiter awaiter;
        awaiter.moveToThread(m_appThread.data());
        QMetaObject::invokeMethod(&awaiter, "call", Q_ARG(std::function<QVariant()>, fx));
        QMutexLocker locker(&awaiter.m_mutex);
        while(!awaiter.m_isFinished) {
            awaiter.m_condition.wait(&awaiter.m_mutex);
        }
        return awaiter.m_ret.value<T>();
    }
    template <typename T>
    QVariant invokeSync(Awaiter &awaiter, const std::function<void()>& fx) {
        awaiter.moveToThread(m_appThread.data());
        QMetaObject::invokeMethod(&awaiter, "callBySignal", Q_ARG(std::function<void()>, fx));
        QMutexLocker locker(&awaiter.m_mutex);
        while(!awaiter.m_isFinished) {
            awaiter.m_condition.wait(&awaiter.m_mutex);
        }
        return awaiter.m_ret.value<T>();
    }
    template <typename R>
    bool connect(const QString &address, int timeout, const std::function<bool()>& waitingFailedCallback) {
        return invokeSync<bool>([=](){
            if(!m_repNode->connectToNode(QUrl(address))) return false;
            m_rep.reset(m_repNode->acquire<R>());
            for(;;) {
                if(!m_rep->waitForSource(timeout)) {
                    if(!waitingFailedCallback()) return false;
                } else break;
            }
            return true;
        });
    }
    template <typename R>
    bool connect(const QString &address, int timeout) {
        return connect<R>(address, timeout, [](){ return false; });
    }
    template <typename R>
    R* replica() {
        return dynamic_cast<R *>(m_rep.data());
    }
private:
    QScopedPointer<QRemoteObjectNode, QScopedPointerDeleteLater> m_repNode;
    QScopedPointer<QRemoteObjectReplica, QScopedPointerDeleteLater> m_rep;
    QScopedPointer<QCoreApplication, QScopedPointerDeleteLater> m_app;
    QScopedPointer<QThread, QScopedPointerDeleteLater> m_appThread;
};

#endif // CHORUSKIT_COMMUNICATIONHELPER_H
