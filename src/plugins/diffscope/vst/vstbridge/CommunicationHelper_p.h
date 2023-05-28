#ifndef CHORUSKIT_COMMUNICATIONHELPER_P_H
#define CHORUSKIT_COMMUNICATIONHELPER_P_H

#include <QMutex>
#include <QVariant>
#include <QWaitCondition>

#include "CommunicationHelper.h"

namespace Vst {

    class AwaiterPrivate {
        Q_DECLARE_PUBLIC(Awaiter)
        Awaiter *q_ptr;
        bool isFinished = false;
        QVariant ret;
        QMutex mutex;
        QWaitCondition condition;
        void setRet(const QVariant &val);
        void finish();
    };

    class CommunicationHelperPrivate {
        Q_DECLARE_PUBLIC(CommunicationHelper)
        CommunicationHelper *q_ptr;
        QScopedPointer<QCoreApplication> app;
        void init();
    };

}
#endif // CHORUSKIT_COMMUNICATIONHELPER_P_H
