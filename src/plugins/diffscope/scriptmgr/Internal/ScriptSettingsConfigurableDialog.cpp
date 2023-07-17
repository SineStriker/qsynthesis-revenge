//
// Created by Crs_1 on 2023/7/17.
//

#include "ScriptSettingsConfigurableDialog.h"

namespace ScriptMgr::Internal {
    ScriptSettingsConfigurableDialog::ScriptSettingsConfigurableDialog(QWidget *parent) : CConfigurableDialog(parent), m_page(new ScriptSettingsPage(this)) {
        setWidget(m_page->widget());
        setWindowTitle(tr("Script Settings"));
        connect(this, &ScriptSettingsConfigurableDialog::applied, m_page, &ScriptSettingsPage::accept);
        connect(this, &ScriptSettingsConfigurableDialog::finished, m_page, &ScriptSettingsPage::finish);
    }

    ScriptSettingsConfigurableDialog::~ScriptSettingsConfigurableDialog() {
    }
}