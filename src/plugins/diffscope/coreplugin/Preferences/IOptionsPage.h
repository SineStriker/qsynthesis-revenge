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
        virtual QString id() const = 0;
        virtual QString title() const = 0;

        virtual QWidget *widget() = 0;
        virtual void reloadStrings() = 0;
        virtual bool match(const QString &pattern) const = 0;

    signals:
        void accepted();
        void done(int result);

    public slots:
        virtual void load() = 0;
        virtual void accept() = 0;
        virtual void finish() = 0;
    protected:
        NodeType nodeType() override { return Page; }
    };
}

#endif // CHORUSKIT_IOPTIONSPAGE_H
