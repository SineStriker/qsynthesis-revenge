//
// Created by Crs_1 on 2023/5/16.
//

#include "CommunicationHelper.h"

static int _appArgc = 0;
static char _appArgv[1] = "";
CommunicationHelper::CommunicationHelper(): m_app(new QCoreApplication(_appArgc, (char**)_appArgv)), m_repNode(new QRemoteObjectNode), m_appThread(new QThread) {
    m_app->moveToThread(m_appThread.data());
    m_repNode->moveToThread(m_appThread.data());
    //QMetaObject::invokeMethod(m_app.data(), &QCoreApplication::exec);
}
void CommunicationHelper::start() {
    m_appThread->start();
}
CommunicationHelper::~CommunicationHelper() {
    m_app->quit();
    m_appThread->quit();
}
void Awaiter::call(std::function<QVariant()> fx) {
    QMutexLocker locker(&m_mutex);
    m_ret = fx();
    m_isFinished = true;
    m_condition.wakeAll();
}
void Awaiter::callBySignal(std::function<void()> fx) {
    m_mutex.lock();
    fx();
}
void Awaiter::finished(const QVariant &val) {
    m_ret = val;
    m_isFinished = true;
    m_condition.wakeAll();
    m_mutex.unlock();
}