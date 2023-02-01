#include "CStartupInfo.h"

#include "private/QMetaTypeImpl_p.h"

#include "CDecorator.h"

CStartupInfo::CStartupInfo(QObject *parent) : QsCoreStartInfo(parent) {
    Register_QMetaTypeImpl();
}

CStartupInfo::~CStartupInfo() {
}

QsCoreDecorator *CStartupInfo::createDecorator(QObject *parent) {
    return new CDecorator(parent);
}
