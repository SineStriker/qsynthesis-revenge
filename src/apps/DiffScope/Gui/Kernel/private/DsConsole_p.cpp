#include "DsConsole_p.h"

#include "CDecorator.h"

DsConsolePrivate::DsConsolePrivate() {
}

DsConsolePrivate::~DsConsolePrivate() {
}

void DsConsolePrivate::init() {
    Q_Q(DsConsole);
    qIDec->installLocale(q, {"DsGui"}, _LOC(DsConsole, q));
}
