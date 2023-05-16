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
    QVariant invokeSync(const std::function<QVariant()>& fx);
    QVariant invokeSync(Awaiter &awaiter, const std::function<void()>& fx);
    template <typename R>
    bool connect(const QString &address) {
        Awaiter awaiter;
        return invokeSync(awaiter, [=, &awaiter](){
            if(!m_repNode->connectToNode(QUrl(address))) emit awaiter.finished(true);
            m_rep.reset(m_repNode->acquire<R>());
            QObject::connect(m_rep.data(), &QRemoteObjectReplica::initialized, [&awaiter](){
                emit awaiter.finished(true);
            });
        }).toBool();
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
