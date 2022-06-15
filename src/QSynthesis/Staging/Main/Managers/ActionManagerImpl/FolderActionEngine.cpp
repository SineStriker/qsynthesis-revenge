#include "FolderActionEngine.h"
#include "FolderActionEngine_p.h"

FolderActionEngine::FolderActionEngine(QObject *parent)
    : FolderActionEngine(*new FolderActionEnginePrivate(), parent) {
}

FolderActionEngine::~FolderActionEngine() {
}

FolderActionEngine::FolderActionEngine(FolderActionEnginePrivate &d, QObject *parent)
    : ActionManagerEngine(d, parent) {
    d.init();
}

void FolderActionEngine::q_actionTriggered(QAction *action) {
}
