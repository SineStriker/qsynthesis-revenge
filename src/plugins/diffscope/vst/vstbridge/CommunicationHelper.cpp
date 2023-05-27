//
// Created by Crs_1 on 2023/5/16.
//

#include "CommunicationHelper.h"
#include "CommunicationHelper_p.h"

namespace Vst {

    static int _appArgc = 0;
    static char _appArgv[1] = "";

    void CommunicationHelperPrivate::init() {
        Q_Q(CommunicationHelper);
        app.reset(new QCoreApplication(_appArgc, (char **) &_appArgv));
        app->moveToThread(q->m_appThread.data());
    }

    CommunicationHelper::CommunicationHelper() : CommunicationHelper(new CommunicationHelperPrivate) {
    }
    CommunicationHelper::CommunicationHelper(CommunicationHelperPrivate *d)
        : d_ptr(d), m_repNode(new QRemoteObjectNode), m_appThread(new QThread) {
        d->q_ptr = this;
        d->init();
        m_repNode->moveToThread(m_appThread.data());
    }
    void CommunicationHelper::start() {
        m_appThread->start();
    }
    void CommunicationHelper::stop() {
        QCoreApplication::quit();
        m_appThread->quit();
    }
    CommunicationHelper::~CommunicationHelper() {
        Q_D(CommunicationHelper);
        d->app->quit();
        m_appThread->quit();
    }
    QThread *CommunicationHelper::appThread() {
        return m_appThread.data();
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