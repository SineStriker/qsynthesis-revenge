#include "CStartInfo_p.h"
#include "private/QMetaTypeImpl_p.h"

#include "QsCoreConfig.h"

CStartInfoPrivate::CStartInfoPrivate() {
}

CStartInfoPrivate::~CStartInfoPrivate() {
}

void CStartInfoPrivate::init() {
    Register_QMetaTypeImpl();
}

void CStartInfoPrivate::load_helper() {
    qDebug() << "start_info: loading first gui component...";

    dd.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd.loadDefault("QsFramework");
}
