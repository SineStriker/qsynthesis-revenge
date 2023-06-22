#include "ScriptSettingsPage.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QStandardPaths>
#include <QFileDialog>

#include <CoreApi/ILoader.h>

namespace ScriptMgr::Internal {
    ScriptSettingsPage::ScriptSettingsPage(QObject *parent) : Core::ISettingPage("scriptmgr.Script", parent) {
        m_widget = nullptr;
        setTitle([]{ return tr("Script"); });
        setDescription([]{ return tr("Manage script files."); });
    }
    ScriptSettingsPage::~ScriptSettingsPage() {
    }
    QString ScriptSettingsPage::sortKeyword() const {
        return "script";
    }
    bool ScriptSettingsPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }
    QWidget *ScriptSettingsPage::widget() {
        if(m_widget) return m_widget;
        auto groupBox =  new QGroupBox;
        groupBox->setTitle(tr("User scripts"));
        m_enableUserScriptsCheckBox = new QCheckBox(tr("Enable user scripts"));
        m_userScriptsDirLabel = new QLineEdit;
        m_userScriptsDirLabel->setReadOnly(true);
        auto browseFileButton = new QPushButton(tr("Browse"));

        auto browseFileLayout = new QHBoxLayout;
        browseFileLayout->addWidget(m_userScriptsDirLabel);
        browseFileLayout->addWidget(browseFileButton);
        auto formLayout = new QFormLayout;
        formLayout->addRow(m_enableUserScriptsCheckBox);
        formLayout->addRow(tr("Directory"), browseFileLayout);
        groupBox->setLayout(formLayout);

        m_enableUserScriptsCheckBox->setChecked(storedEnableUserScripts());
        m_userScriptsDirLabel->setText(storedUserScriptsDirectory());
        connect(browseFileButton, &QPushButton::clicked, this, [=](){
            auto dir = QFileDialog::getExistingDirectory(groupBox, tr("Select User Scripts Directory"), m_userScriptsDirLabel->text());
            if(!dir.isEmpty()) m_userScriptsDirLabel->setText(QDir::toNativeSeparators(dir));
        });
        connect(m_enableUserScriptsCheckBox, &QCheckBox::clicked, this, [=](bool checked){
           browseFileButton->setDisabled(!checked);
           m_userScriptsDirLabel->setDisabled(!checked);
        });

        return m_widget = groupBox;
    }
    bool ScriptSettingsPage::accept() {
        QDir dir(m_userScriptsDirLabel->text());
        if(!dir.exists()) return false;
        QJsonObject obj;
        obj.insert("enableUserScripts", m_enableUserScriptsCheckBox->isChecked());
        obj.insert("userScriptsDirectory", m_userScriptsDirLabel->text());
        Core::ILoader::instance()->settings()->insert("ScriptMgr", obj);
        return true;
    }
    void ScriptSettingsPage::finish() {
        if(m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

    bool ScriptSettingsPage::storedEnableUserScripts() {
        auto enableUserScripts = Core::ILoader::instance()->settings()->value("ScriptMgr").toObject().value("enableUserScripts");
        if(enableUserScripts.isBool()) {
            return enableUserScripts.toBool();
        } else {
            return true;
        }
    }

    QString ScriptSettingsPage::storedUserScriptsDirectory() {
        auto directory = Core::ILoader::instance()->settings()->value("ScriptMgr").toObject().value("userScriptsDirectory");
        if(false) {
            return directory.toString();
        } else {
            auto defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ChorusKit/DiffScope/User Scripts";
            if(!QDir(defaultPath).exists()) {
                QDir().mkpath(defaultPath);
            }
            return QDir::toNativeSeparators(defaultPath);
        }
    }
} // Internal