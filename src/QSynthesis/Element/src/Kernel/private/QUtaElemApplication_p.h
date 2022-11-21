#ifndef __QUTAELEMAPPLICATION_P_H__
#define __QUTAELEMAPPLICATION_P_H__

#include "../QUtaElemApplication.h"
#include "Kernel/private/QsApplication_p.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

#include "../QUtaDistConfig.h"

class QUTAELEM_API QUtaElemApplicationPrivate : public QsApplicationPrivate {
    Q_DECLARE_PUBLIC(QUtaElemApplication)
public:
    QUtaElemApplicationPrivate();
    ~QUtaElemApplicationPrivate();

    void init();
    void deinit();

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // __QUTAELEMAPPLICATION_P_H__