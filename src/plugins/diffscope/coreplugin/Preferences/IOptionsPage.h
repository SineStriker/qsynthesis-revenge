//
// Created by Crs_1 on 2023/4/5.
//

#ifndef CHORUSKIT_IOPTIONSPAGE_H
#define CHORUSKIT_IOPTIONSPAGE_H

#include "CoreApi/IDialogPage.h"
#include "IPreferenceItem.h"

namespace Core {
    class IOptionsPage: public IDialogPage, public IPreferenceItem {
        Q_OBJECT
    public:
        virtual bool match(const QString &pattern) const = 0;
    protected:
        NodeType nodeType() override { return Page; }
    };
}

#endif // CHORUSKIT_IOPTIONSPAGE_H
