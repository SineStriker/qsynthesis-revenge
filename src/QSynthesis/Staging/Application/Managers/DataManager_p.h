#ifndef DATAMANAGERPRIVATE_H
#define DATAMANAGERPRIVATE_H

#include "DataManager.h"
#include "Vendor/Objects/BasicManager_p.h"

class DataManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(DataManager)
public:
    DataManagerPrivate();
    virtual ~DataManagerPrivate();

    void init();
};

#endif // DATAMANAGERPRIVATE_H
