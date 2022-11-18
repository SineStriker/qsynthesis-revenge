#include "CApplication_p.h"
#include "QMetaTypeImpl.h"

static const SingleApplication::Options opts = SingleApplication::ExcludeAppPath |
                                               SingleApplication::ExcludeAppVersion |
                                               SingleApplication::SecondaryNotification;

CApplicationPrivate::CApplicationPrivate() : hMSH(new MultistyleHandle()) {
}

void CApplicationPrivate::init() {
    Q_Q(CApplication);

    QMetaTypeImpl::Register();
}

void CApplicationPrivate::deinit() {
    Q_Q(CApplication);
    hSingle.reset();
}

void CApplicationPrivate::setupSingle() {
    Q_Q(CApplication);

    hSingle.reset(new SingleApplication(q, true, opts));

    if (!hSingle->isPrimary()) {
        qInfo() << "Primary instance already running. PID:" << q->primaryPid();

        // This eventually needs moved into the NotepadNextApplication to keep
        // sending/receiving logic in the same place
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::WriteOnly);

        stream << q->arguments();
        hSingle->sendMessage(buffer);

        qInfo() << "Secondary instance closing...";

        q->exit(0);

        ::exit(0);
    }
}

void CApplicationPrivate::instanceStarted_helper() {
}

void CApplicationPrivate::messageReceived_helper(const QStringList &args) {
    Q_UNUSED(args);
}
