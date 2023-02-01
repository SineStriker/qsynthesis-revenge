#include "QsStartInfo_p.h"

QsStartInfoPrivate::QsStartInfoPrivate() {
}

QsStartInfoPrivate::~QsStartInfoPrivate() {
    deinit();
}

void QsStartInfoPrivate::init() {
}

void QsStartInfoPrivate::parse_helper() {
    Q_Q(QsStartInfo);
    dd.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd.loadDefault("QsIntegrate");

    // Init managers
    pluginMgr = new QsPluginManager(q);
    fileMgr = new QsFileManager(q);

    fileMgr->load();
    pluginMgr->load();
}

void QsStartInfoPrivate::deinit() {
    // Save Modules
    pluginMgr->save();
    fileMgr->save();

    delete pluginMgr;
    delete fileMgr;
}