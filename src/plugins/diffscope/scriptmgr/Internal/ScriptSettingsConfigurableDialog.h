//
// Created by Crs_1 on 2023/7/17.
//

#ifndef CHORUSKIT_SCRIPTSETTINGSCONFIGURABLEDIALOG_H
#define CHORUSKIT_SCRIPTSETTINGSCONFIGURABLEDIALOG_H

#include "CConfigurableDialog.h"

#include "ScriptSettingsPage.h"

namespace ScriptMgr::Internal {

    class ScriptSettingsConfigurableDialog : public CConfigurableDialog {
        ScriptSettingsPage *m_page;
    public:
        ScriptSettingsConfigurableDialog(QWidget *parent = nullptr);
        ~ScriptSettingsConfigurableDialog();
    };

}

#endif // CHORUSKIT_SCRIPTSETTINGSCONFIGURABLEDIALOG_H
