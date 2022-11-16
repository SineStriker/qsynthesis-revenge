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

    q->connect(q, &SingleApplication::instanceStarted, q, &CApplication::_q_instanceStarted);
    q->connect(q, &SingleApplication::receivedMessage, q, &CApplication::_q_messageReceived);

    q->connect(hMSH.data(), &MultistyleHandle::localeChanged, q, &CApplication::_q_localeChanged);
    q->connect(hMSH.data(), &MultistyleHandle::themeChanged, q, &CApplication::_q_themeChanged);
}

void CApplicationPrivate::instanceStarted_helper() {
}

void CApplicationPrivate::messageReceived_helper(const QStringList &args) {
    Q_UNUSED(args);
}
