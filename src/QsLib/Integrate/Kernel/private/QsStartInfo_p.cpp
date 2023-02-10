#include "QsStartInfo_p.h"

#include "CDecorator.h"

QsStartInfoPrivate::QsStartInfoPrivate() {
}

QsStartInfoPrivate::~QsStartInfoPrivate() {
}

void QsStartInfoPrivate::init() {
}

void QsStartInfoPrivate::load_helper() {
    qIDec->addThemeTemplate("ImportDialog", ":/themes/import-dialog.qss.in");

    dd.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd.loadDefault("QsIntegrate");
}
