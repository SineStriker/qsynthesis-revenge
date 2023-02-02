#include "DsStartInfo.h"
#include "private/DsStartInfo_p.h"

#include "DsDistConfig.h"

#include <QApplication>

DsStartInfo::DsStartInfo(QObject *parent) : DsStartInfo(*new DsStartInfoPrivate(), parent) {
    appDescription = "DiffSinger editor maintained by OpenVPI";
}

DsStartInfo::~DsStartInfo() {
}

void DsStartInfo::parse() {
    Q_D(DsStartInfo);

    qApp->setApplicationName(APP_NAME);
    qApp->setApplicationVersion(APP_VERSION);
    qApp->setApplicationDisplayName(APP_NAME);

    QsStartInfo::parse();

    d->parse_helper();
}

QsCoreConfig *DsStartInfo::creatDistConfig() {
    return new DsDistConfig();
}

DsStartInfo::DsStartInfo(DsStartInfoPrivate &d, QObject *parent) : QsStartInfo(d, parent) {
    d.init();
}
