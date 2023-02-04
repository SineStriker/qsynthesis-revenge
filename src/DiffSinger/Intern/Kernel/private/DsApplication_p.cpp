#include "DsApplication_p.h"

#include <QDebug>

DsApplicationPrivate::DsApplicationPrivate() {
}

DsApplicationPrivate::~DsApplicationPrivate() {
    deinit();
}

void DsApplicationPrivate::init() {
    Q_Q(DsApplication);
    startInfo = new DsStartInfo(q);
    startInfo->load();
}

void DsApplicationPrivate::deinit() {
    delete startInfo;
}
