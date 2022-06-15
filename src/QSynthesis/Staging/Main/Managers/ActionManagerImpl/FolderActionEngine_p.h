#ifndef FOLDERACTIONENGINEPRIVATE_H
#define FOLDERACTIONENGINEPRIVATE_H

#include "ActionManagerEngine_p.h"

#include "FolderActionEngine.h"

class FolderActionEnginePrivate : public ActionManagerEnginePrivate {
    Q_DECLARE_PUBLIC(FolderActionEngine)
public:
    FolderActionEnginePrivate();
    ~FolderActionEnginePrivate();

    void init();
};

#endif // FOLDERACTIONENGINEPRIVATE_H
