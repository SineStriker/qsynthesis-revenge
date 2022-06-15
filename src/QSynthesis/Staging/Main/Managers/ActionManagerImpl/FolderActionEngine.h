#ifndef FOLDERACTIONENGINE_H
#define FOLDERACTIONENGINE_H

#include "ActionManagerEngine.h"

class FolderActionEnginePrivate;

class FolderActionEngine : public ActionManagerEngine {
    Q_DECLARE_PRIVATE(FolderActionEngine)
public:
    explicit FolderActionEngine(QObject *parent = nullptr);
    ~FolderActionEngine();

protected:
    FolderActionEngine(FolderActionEnginePrivate &d, QObject *parent = nullptr);

private:
    void q_actionTriggered(QAction *action);
};
#endif // FOLDERACTIONENGINE_H
