#include "DsStartInfo.h"

#include <QApplication>

#include "Managers/WindowManager.h"
#include "QsView.h"


DsStartInfo::DsStartInfo() {
    parser.addPositionalArgument("files", "Project files to open.", "files");
}

DsStartInfo::~DsStartInfo() {
}

void DsStartInfo::receiveMessage(quint32 instanceId, const QByteArray &message) {
    Q_UNUSED(instanceId);
    Q_UNUSED(message);
    QsView::bringWindowToForeground(qWindows->firstWindow());
}
