//
// Created by Crs_1 on 2023/5/16.
//

#include "CommunicationHelper.h"
#include "CommunicationHelper_p.h"

namespace Vst {

    static int _appArgc = 0;
    static char _appArgv[1] = "";

    QScopedPointer<CommunicationHelper::AppContainer> CommunicationHelper::m_container;

    CommunicationHelper::AppContainer::~AppContainer() {
        appThread->quit();
        appThread->wait();
    }

    CommunicationHelper::CommunicationHelper()
        : m_repNode(new QRemoteObjectNode) {
        if(m_container.isNull()) {
            m_container.reset(new AppContainer);
            m_container->appThread.reset(new QThread);
            m_container->app.reset(new QCoreApplication(_appArgc, (char **) &_appArgv));
            m_container->app->moveToThread(m_container->appThread.data());
        }
        m_repNode->moveToThread(m_container->appThread.data());
    }
    void CommunicationHelper::start() {
        m_container->appThread->start();
    }
    void CommunicationHelper::stop() {
        QCoreApplication::quit();
        m_container->appThread->quit();
    }
    CommunicationHelper::~CommunicationHelper() {

    }
    QThread *CommunicationHelper::appThread() {
        return m_container->appThread.data();
    }

    void AwaiterPrivate::setRet(const QVariant &val) {
        ret = val;
    }
    void AwaiterPrivate::finish() {
        isFinished = true;
        condition.wakeAll();
    }

    void Awaiter::call(std::function<QVariant()> fx) {
        Q_D(Awaiter);
        QMutexLocker locker(&d->mutex);
        d->setRet(fx());
        d->finish();
    }
    void Awaiter::callBySignal(std::function<void()> fx) {
        Q_D(Awaiter);
        d->mutex.lock();
        fx();
    }
    void Awaiter::finish(const QVariant &val) {
        Q_D(Awaiter);
        d->setRet(val);
        d->finish();
        d->mutex.unlock();
        emit finished(val);
    }
    void Awaiter::wait() {
        Q_D(Awaiter);
        QMutexLocker locker(&d->mutex);
        while (!d->isFinished) {
            d->condition.wait(&d->mutex);
        }
    }
    QVariant Awaiter::returnValue() {
        Q_D(Awaiter);
        return d->ret;
    }
    Awaiter::Awaiter() : Awaiter(new AwaiterPrivate) {
    }
    Awaiter::Awaiter(AwaiterPrivate *d) : d_ptr(d) {
        d->q_ptr = this;
    }
    Awaiter::~Awaiter() noexcept {

    }

}