#ifndef QSSTARTINFOPRIVATE_H
#define QSSTARTINFOPRIVATE_H

#include "../QsStartInfo.h"
#include "private/CStartInfo_p.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "CDecorateDir.h"

class QSINTEGRATE_API QsStartInfoPrivate : public CStartInfoPrivate {
    Q_DECLARE_PUBLIC(QsStartInfo)
public:
    QsStartInfoPrivate();
    ~QsStartInfoPrivate();

    void init();
    void parse_helper();
    void deinit();

    // Managers
    QsPluginManager *pluginMgr;
    QsFileManager *fileMgr;

private:
    CDecorateDir dd;
};

#endif // QSSTARTINFOPRIVATE_H
