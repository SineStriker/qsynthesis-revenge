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
    void load_helper();

private:
    CDecorateDir dd;
};

#endif // QSSTARTINFOPRIVATE_H
