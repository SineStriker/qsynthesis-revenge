#ifndef __DSELEMAPPLICATION_P_H__
#define __DSELEMAPPLICATION_P_H__

#include "../DsElemApplication.h"
#include "Kernel/private/QsApplication_p.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

#include "../DsDistConfig.h"

class DSELEM_API DsElemApplicationPrivate : public QsApplicationPrivate {
    Q_DECLARE_PUBLIC(DsElemApplication)
public:
    DsElemApplicationPrivate();
    ~DsElemApplicationPrivate();

    void init();
    void deinit();

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // __DSELEMAPPLICATION_P_H__
