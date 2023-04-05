//
// Created by Crs_1 on 2023/4/3.
//

#ifndef CHORUSKIT_DIALOGHELPER_P_H
#define CHORUSKIT_DIALOGHELPER_P_H

#include "DialogHelper.h"

#include <QSet>

namespace Core {

    class DialogHelperPrivate: public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(DialogHelper)
    public:
        DialogHelper* q_ptr;
        QMap<QString, IDialogPage *> dialogs;
        QSet<IDialogPage *> openedDialogPages;
    };

}

#endif // CHORUSKIT_DIALOGHELPER_P_H
