#ifndef DOCUMENTTABPRIVATE_H
#define DOCUMENTTABPRIVATE_H

#include "../CentralTab_p.h"
#include "DocumentTab.h"

class DocumentTabPrivate : public CentralTabPrivate {
    Q_DECLARE_PUBLIC(DocumentTab)
public:
    DocumentTabPrivate();
    ~DocumentTabPrivate();

    void init();

    bool deleted;
    bool untitled;

    QString setTabNameProxy(const QString &tabName) override;
    QString tabText() override;
};

#endif // DOCUMENTTABPRIVATE_H
