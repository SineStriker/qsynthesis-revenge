#ifndef FOLDERTABPRIVATE_H
#define FOLDERTABPRIVATE_H

#include "../CentralTab_p.h"

#include "FolderTab.h"

class FolderTabPrivate : public CentralTabPrivate {
    Q_DECLARE_PUBLIC(FolderTab)
public:
    FolderTabPrivate();
    ~FolderTabPrivate();

    void init();

    bool unreachable;

    QString tabText() override;
};

#endif // FOLDERTABPRIVATE_H
