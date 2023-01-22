#ifndef QSELEMAPPLICATIONPRIVATE_H
#define QSELEMAPPLICATIONPRIVATE_H

#include "../QsApplication.h"
#include "private/CApplication_p.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "Tools/LocalLinguist.h"

#include "../QsDistConfig.h"

#include <QCommandLineParser>

class QSINTEGRATE_API QsApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate(QsDistConfig *conf = nullptr);
    ~QsApplicationPrivate();

    void init();
    void deinit();

    QsApplication *q_ptr;

    // CMD parser
    QCommandLineParser parser;

    // App Config
    QScopedPointer<QsDistConfig> conf;

    // Managers
    QsPluginManager *pluginMgr;
    QsFileManager *fileMgr;

private:
    LocalLinguist *ll;
};

#endif // QSELEMAPPLICATIONPRIVATE_H
