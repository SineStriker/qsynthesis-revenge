//
// Created by Crs_1 on 2023/5/16.
//

#ifndef CHORUSKIT_COMMUNICATIONHELPER_H
#define CHORUSKIT_COMMUNICATIONHELPER_H

#include <QtRemoteObjects>

Q_DECLARE_METATYPE(std::function<QVariant()>)
Q_DECLARE_METATYPE(std::function<void()>)

namespace Vst {

    class AwaiterPrivate;

    class Awaiter : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Awaiter)
        friend class CommunicationHelper;

    public:
        explicit Awaiter();
        ~Awaiter();
        Q_INVOKABLE void call(std::function<QVariant()> fx);
        Q_INVOKABLE void callBySignal(std::function<void()> fx);
        void wait();
        QVariant returnValue();
    signals:
        void finished(const QVariant &val);
    public slots:
        void finish(const QVariant &val);

    private:
        explicit Awaiter(AwaiterPrivate *d);
        QScopedPointer<AwaiterPrivate> d_ptr;
    };

    class CommunicationHelper {
    public:
        explicit CommunicationHelper();
        ~CommunicationHelper();
        void start();
        void stop();

        template <typename T>
        T invokeSync(const std::function<T()> &fx) {
            Awaiter awaiter;
            awaiter.moveToThread(m_container->appThread.data());
            QMetaObject::invokeMethod(&awaiter, "call", Q_ARG(std::function<QVariant()>, fx));
            awaiter.wait();
            return awaiter.returnValue().value<T>();
        }

        template <typename T>
        void invokeAsync(const std::function<T()> &fx, const std::function<void(const T &)> &callback) {
            auto awaiter = new Awaiter;
            awaiter->moveToThread(m_container->appThread.data());
            QMetaObject::invokeMethod(awaiter, "callBySignal", Q_ARG(std::function<void()>, [=]() {
                auto ret = fx();
                awaiter->finish(QVariant(ret));
            }));
            QObject::connect(awaiter, &Awaiter::finished, awaiter, [=](QVariant val) {
                callback(val.value<T>());
                awaiter->deleteLater();
            });
        }

        template <typename T>
        QVariant invokeSync(Awaiter &awaiter, const std::function<void()> &fx) {
            awaiter.moveToThread(m_container->appThread.data());
            QMetaObject::invokeMethod(&awaiter, "callBySignal", Q_ARG(std::function<void()>, fx));
            awaiter.wait();
            return awaiter.returnValue().value<T>();
        }
        template <typename R>
        bool connect(const QString &address, int timeout) {
            return invokeSync<bool>([=]() {
                if (!m_repNode->connectToNode(QUrl(address)))
                    return false;
                m_rep.reset(m_repNode->acquire<R>());
                return m_rep->waitForSource(timeout);
            });
        }
        template <typename R>
        void connectAsync(const QString &address, const std::function<void(bool)> &callback) {
            auto awaiter = new Awaiter;
            awaiter->moveToThread(m_container->appThread.data());
            QMetaObject::invokeMethod(awaiter, "callBySignal", Q_ARG(std::function<void()>, [=]() {
                if(!m_repNode->connectToNode(QUrl(address))) {
                    awaiter->finish(false);
                }
                m_rep.reset(m_repNode->acquire<R>());
                auto _ = new QObject;
                QObject::connect(m_rep.data(), &QRemoteObjectReplica::stateChanged, _, [=](QRemoteObjectReplica::State state){
                    delete _;
                    awaiter->finish(state == QRemoteObjectReplica::Valid);
                });
                QObject::connect(awaiter, &Awaiter::finished, awaiter, [=](QVariant val){
                    callback(val.toBool());
                    awaiter->deleteLater();
                });
            }));
        }
        template <typename R>
        R *replica() {
            return dynamic_cast<R *>(m_rep.data());
        }

        QThread *appThread();

    private:
        QScopedPointer<QRemoteObjectReplica, QScopedPointerDeleteLater> m_rep;
        QScopedPointer<QRemoteObjectNode, QScopedPointerDeleteLater> m_repNode;
        class AppContainer {
        public:
           QScopedPointer<QThread> appThread;
            QScopedPointer<QCoreApplication> app;
            ~AppContainer();
        };
        static QScopedPointer<AppContainer> m_container;
    };

}

#endif // CHORUSKIT_COMMUNICATIONHELPER_H
