//
// Created by Crs_1 on 2023/4/17.
//

#ifndef CHORUSKIT_JSFORMDIALOG_H
#define CHORUSKIT_JSFORMDIALOG_H

#include "Dialogs/ConfigurableDialog.h"

namespace ScriptMgr::Internal {

    class JsFormDialog: public QsApi::ConfigurableDialog {
        Q_OBJECT
    public:
        explicit JsFormDialog(QWidget *parent = nullptr);
        ~JsFormDialog();
    };

} // Internal

#endif // CHORUSKIT_JSFORMDIALOG_H
