#ifndef EVENTMANAGERPRIVATE_H
#define EVENTMANAGERPRIVATE_H

#include "../CentralManager_p.h"

#include "EventManager.h"

class EventManagerPrivate : public CentralManagerPrivate {
    Q_DECLARE_PUBLIC(EventManager)
public:
    EventManagerPrivate();
    ~EventManagerPrivate();

    void init();
};

#endif // EVENTMANAGERPRIVATE_H
