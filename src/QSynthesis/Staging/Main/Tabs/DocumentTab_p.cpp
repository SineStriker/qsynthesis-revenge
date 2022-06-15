#include "DocumentTab_p.h"
#include "SystemHelper.h"

#include "Managers/DataManager.h"

DocumentTabPrivate::DocumentTabPrivate() {
    deleted = false;
    untitled = false;
}

DocumentTabPrivate::~DocumentTabPrivate() {
}

void DocumentTabPrivate::init() {
}

QString DocumentTabPrivate::setTabNameProxy(const QString &tabName) {
    QString newName;
    if (deleted) {
        newName = qData->deletedPrefix() + tabName;
    } else if (edited) {
        newName = CentralTabPrivate::setTabNameProxy(tabName);
    } else {
        newName = tabName;
    }
    return newName;
}

QString DocumentTabPrivate::tabText() {
    return QDir::toNativeSeparators(Sys::PathFindFileName(filename));
}
