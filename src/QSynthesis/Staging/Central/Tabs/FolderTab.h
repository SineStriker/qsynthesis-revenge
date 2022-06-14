#ifndef FOLDERTAB_H
#define FOLDERTAB_H

#include "CentralTab.h"

class FolderTabPrivate;

class FolderTab : public CentralTab {
    Q_OBJECT
    Q_DECLARE_PRIVATE(FolderTab)
public:
    explicit FolderTab(QWidget *parent = nullptr);
    ~FolderTab();

    Type type() const override;

protected:
    FolderTab(FolderTabPrivate &d, QWidget *parent = nullptr);

public:
    bool isUnreachable() const;

protected:
    virtual void setUnreachable(bool unreachable);

signals:
};

#endif // FOLDERTAB_H
