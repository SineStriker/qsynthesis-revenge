#include "AceTreeRecoverable.h"
#include "AceTreeRecoverable_p.h"

AceTreeRecoverablePrivate::AceTreeRecoverablePrivate() {
    m_dev = nullptr;
    m_fileDev = nullptr;
}

AceTreeRecoverablePrivate::~AceTreeRecoverablePrivate() {
}

void AceTreeRecoverablePrivate::init() {
}

void AceTreeRecoverablePrivate::logError() {
    Q_Q(AceTreeRecoverable);
    q->stopLogging();
    emit q->loggingError();
}

AceTreeRecoverable::~AceTreeRecoverable() {
}

void AceTreeRecoverable::startLogging(QIODevice *dev) {
    Q_D(AceTreeRecoverable);
    if (d->m_dev)
        stopLogging();

    d->m_dev = dev;
    d->m_fileDev = qobject_cast<QFileDevice *>(dev); // Try QFileDevice

    d->logStart();
}

void AceTreeRecoverable::stopLogging() {
    Q_D(AceTreeRecoverable);

    d->logStop();

    d->m_dev = nullptr;
    d->m_fileDev = nullptr;
}

bool AceTreeRecoverable::isLogging() const {
    Q_D(const AceTreeRecoverable);
    return d->m_dev;
}

bool AceTreeRecoverable::recover(const QByteArray &data) {
    return false;
}

AceTreeRecoverable::AceTreeRecoverable(AceTreeRecoverablePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
