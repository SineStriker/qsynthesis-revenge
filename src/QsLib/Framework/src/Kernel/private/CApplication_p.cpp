#include "CApplication_p.h"
#include "QMetaTypeImpl.h"

CApplicationPrivate::CApplicationPrivate() : hMSH(new MultistyleHandle()) {
}

void CApplicationPrivate::init() {
    Q_Q(CApplication);

    QMetaTypeImpl::Register();

    if (!q->isPrimary()) {
        qInfo() << "Primary instance already running. PID:" << q->primaryPid();

        // This eventually needs moved into the NotepadNextApplication to keep
        // sending/receiving logic in the same place
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::WriteOnly);

        stream << q->arguments();
        q->sendMessage(buffer);

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
