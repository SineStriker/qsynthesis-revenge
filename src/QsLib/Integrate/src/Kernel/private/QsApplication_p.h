#ifndef QSELEMAPPLICATIONPRIVATE_H
#define QSELEMAPPLICATIONPRIVATE_H

#include "../QsApplication.h"
#include "private/CApplication_p.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

#include "../QsDistConfig.h"

class QSINTEGRATE_API QsApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate(QsDistConfig *conf = nullptr);
    ~QsApplicationPrivate();

    void init();
    void deinit();

    void init2();

    QScopedPointer<QsDistConfig> conf;

    QsPluginManager *pluginMgr;
    QsFileManager *fileMgr;

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // QSELEMAPPLICATIONPRIVATE_H
