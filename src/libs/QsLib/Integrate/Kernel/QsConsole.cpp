#include "QsConsole.h"
#include "private/QsConsole_p.h"

QsConsole::QsConsole(QObject *parent) : QsConsole(*new QsConsolePrivate(), parent) {
}

QsConsole::~QsConsole() {
}

QsConsole::QsConsole(QsConsolePrivate &d, QObject *parent) : CConsole(d, parent) {
    d.init();
}