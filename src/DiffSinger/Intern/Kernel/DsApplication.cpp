#include "DsApplication.h"

#include "DsDistConfig.h"
#include "private/DsApplication_p.h"

#include "Kernel/DsStartInfo.h"
#include "QsView.h"

#include <QScreen>

DsApplication::DsApplication(int &argc, char **argv)
    : DsApplication(*new DsApplicationPrivate(), argc, argv) {
}

DsApplication::~DsApplication() {
    Q_D(DsApplication);
    d->deinit();
}

DsApplication::DsApplication(DsApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
