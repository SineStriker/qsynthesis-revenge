#ifndef ACETREERECOVERABLEPRIVATE_H
#define ACETREERECOVERABLEPRIVATE_H

#include <QFileDevice>

#include "AceTreeRecoverable.h"

class AceTreeRecoverablePrivate {
    Q_DECLARE_PUBLIC(AceTreeRecoverable)
public:
    AceTreeRecoverablePrivate();
    virtual ~AceTreeRecoverablePrivate();

    void init();

    AceTreeRecoverable *q_ptr;

    QIODevice *m_dev;
    QFileDevice *m_fileDev;

    virtual void logStart() = 0;
    virtual void logStop() = 0;

    void logError();
};

#endif // ACETREERECOVERABLEPRIVATE_H