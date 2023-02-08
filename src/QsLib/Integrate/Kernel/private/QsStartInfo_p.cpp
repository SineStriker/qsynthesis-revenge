#include "QsStartInfo_p.h"

#include "CDecorator.h"

QsStartInfoPrivate::QsStartInfoPrivate() {
}

QsStartInfoPrivate::~QsStartInfoPrivate() {
    deinit();
}

void QsStartInfoPrivate::init() {
}

void QsStartInfoPrivate::load_helper() {
    Q_Q(QsStartInfo);
    qIDec->addThemeTemplate("ImportDialog", ":/themes/import-dialog.qss.in");

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
