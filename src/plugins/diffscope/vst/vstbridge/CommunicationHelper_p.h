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

}
#endif // CHORUSKIT_COMMUNICATIONHELPER_P_H
