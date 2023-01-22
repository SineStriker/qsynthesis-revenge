#include "DsApplication.h"

#include "DsDistConfig.h"
#include "private/DsApplication_p.h"

#include "Kernel/DsStartupInfo.h"
#include "QsView.h"

#include <QScreen>

DsApplication::DsApplication(int &argc, char **argv)
    : DsApplication(*new DsApplicationPrivate(), argc, argv) {
}

DsApplication::~DsApplication() {
    Q_D(DsApplication);
    d->deinit();
}

void DsApplication::receiveMessage(quint32 instanceId, const QByteArray &message) {
    Q_UNUSED(instanceId);
    Q_UNUSED(message);

    Q_D(DsApplication);
    QsView::bringWindowToForeground(d->windowMgr->firstWindow());
}

DsApplication::DsApplication(DsApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
