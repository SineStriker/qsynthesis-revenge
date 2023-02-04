#include "CStartInfo.h"
#include "private/CStartInfo_p.h"

#include "CConsole.h"
#include "CDecorator.h"

CStartInfo::CStartInfo(QObject *parent) : CStartInfo(*new CStartInfoPrivate(), parent) {
}

CStartInfo::~CStartInfo() {
}

QsCoreConsole *CStartInfo::createConsole(QObject *parent) {
    return new CConsole(parent);
}

QsCoreDecorator *CStartInfo::createDecorator(QObject *parent) {
    return new CDecorator(parent);
}

CStartInfo::CStartInfo(CStartInfoPrivate &d, QObject *parent) : QsCoreStartInfo(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
