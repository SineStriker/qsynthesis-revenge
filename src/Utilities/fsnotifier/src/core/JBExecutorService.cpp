#include "JBExecutorService.h"

JBExecutorService::JBExecutorService(QObject *parent) : QObject(parent) {
}

JBExecutorService::~JBExecutorService() {
}

QThread *JBExecutorService::taskThread() const {
    return m_taskThread;
}
