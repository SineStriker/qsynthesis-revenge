#include "QMGuiConsole.h"
#include "private/QMGuiConsole_p.h"

QMGuiConsolePrivate::QMGuiConsolePrivate() {
}

QMGuiConsolePrivate::~QMGuiConsolePrivate() {
}

void QMGuiConsolePrivate::init() {
}

QMGuiConsole::QMGuiConsole(QObject *parent) : QMGuiConsole(*new QMGuiConsolePrivate(), parent) {
}

QMGuiConsole::~QMGuiConsole() {
}

QMGuiConsole::QMGuiConsole(QMGuiConsolePrivate &d, QObject *parent) : QMCoreConsole(d, parent) {
    d.init();
}