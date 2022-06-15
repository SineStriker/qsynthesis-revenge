#include "ActionManagerEngine.h"
#include "ActionManagerEngine_p.h"

ActionManagerEngine::ActionManagerEngine(QObject *parent)
    : ActionManagerEngine(*new ActionManagerEnginePrivate(), parent) {
}

ActionManagerEngine::~ActionManagerEngine() {
}

void ActionManagerEngine::setup() {
    Q_D(ActionManagerEngine);
    d->setup();

    d->reloadStrings();
    d->reloadShortcuts();
}

void ActionManagerEngine::reloadStrings() {
    Q_D(ActionManagerEngine);
    d->reloadStrings();
}

void ActionManagerEngine::reloadShortcuts() {
    Q_D(ActionManagerEngine);
    d->reloadShortcuts();
}

ActionManagerEngine::ActionManagerEngine(ActionManagerEnginePrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
