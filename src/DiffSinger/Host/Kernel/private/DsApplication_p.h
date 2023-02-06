#ifndef DSAPPLICATIONPRIVATE_H
#define DSAPPLICATIONPRIVATE_H

#include "DsApplication.h"
#include "DsLocalData.h"
#include "DsStartInfo.h"

class DsApplicationPrivate {
    Q_DECLARE_PUBLIC(DsApplication)
public:
    DsApplicationPrivate();
    virtual ~DsApplicationPrivate();

    void init();
    void deinit();

    DsApplication *q_ptr;

    DsStartInfo *startInfo; // QsCoreStartInfo instance
    DsLocalData *localData;
};

#endif // DSAPPLICATIONPRIVATE_H
