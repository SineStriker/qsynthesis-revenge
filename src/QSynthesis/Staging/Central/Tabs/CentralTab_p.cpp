#include "CentralTab_p.h"

#include "DataManager.h"

CentralTabPrivate::CentralTabPrivate() {
    edited = false;
}

CentralTabPrivate::~CentralTabPrivate() {
}

void CentralTabPrivate::init() {
}

QString CentralTabPrivate::setTabNameProxy(const QString &tabName) {
    return (edited ? qData->untitledPrefix() : "") + tabName;
}

QString CentralTabPrivate::tabText() {
    return QString();
}
