#include "DsStartInfo.h"
#include "private/DsStartInfo_p.h"

#include "DsDistConfig.h"

DsStartInfo::DsStartInfo(QObject *parent) : DsStartInfo(*new DsStartInfoPrivate(), parent) {
}

DsStartInfo::~DsStartInfo() {
}

void DsStartInfo::parse() {
    Q_D(DsStartInfo);
    QsStartInfo::parse();
    d->parse_helper();
}

QsCoreConfig *DsStartInfo::creatDistConfig() {
    return new DsDistConfig();
}

DsStartInfo::DsStartInfo(DsStartInfoPrivate &d, QObject *parent) : QsStartInfo(d, parent) {
    d.init();
}
