//
// Created by Crs_1 on 2023/4/3.
//

#ifndef CHORUSKIT_IDIALOGPAGE_H
#define CHORUSKIT_IDIALOGPAGE_H

#include "CkCoreGlobal.h"
#include <QObject>

namespace Core {

    class CKCORE_API IDialogPage: public QObject {
        Q_OBJECT
    public:
        virtual QString id() const = 0;
        virtual QString title() const = 0;

        virtual QWidget *widget() = 0;
        virtual void reloadStrings() = 0;

    signals:
        void accepted();
        void done(int result);

    public slots:
        virtual void load() = 0;
        virtual void accept() = 0;
        virtual void finish() = 0;
    };
}

#endif // CHORUSKIT_IDIALOGPAGE_H
