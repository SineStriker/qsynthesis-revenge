#include "DsApplication_p.h"

#include <QDebug>

#include "DsDistConfig.h"
#include "IDspxPorter.h"
#include "QsPluginManager.h"

static const char Slash = '/';

static const char LocalDataFile[] = "local.json";

DsApplicationPrivate::DsApplicationPrivate() {
}

DsApplicationPrivate::~DsApplicationPrivate() {
    deinit();
}

void DsApplicationPrivate::init() {
    Q_Q(DsApplication);

    q->setApplicationName(APP_NAME);
    q->setApplicationVersion(APP_VERSION);
    // q->setApplicationDisplayName(APP_NAME);

    startInfo = new DsStartInfo(q);
    startInfo->load();

    localData = new DsLocalData();
    localData->load(qAppConf->appDir(DsDistConfig::AppData) + Slash + LocalDataFile);

    qsPluginMgr->addPluginSet("dspxporters",
                              {
                                  qAppConf->appDir(DsDistConfig::AppPlugins) + "/dspxporters",
                                  std::bind(IDspxPorter::categoryName),
                              });
}

void DsApplicationPrivate::deinit() {
    localData->save(qAppConf->appDir(DsDistConfig::AppData) + Slash + LocalDataFile);
    delete localData;

    startInfo->save();
    delete startInfo;
}
