#include "ScriptSettingsPage.h"

#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStandardPaths>
#include <QTreeWidget>

#include <CoreApi/ILoader.h>

#include "ScriptLoader.h"
#include "JsMacros.h"

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
        auto mainWidget = new QWidget;
        auto mainLayout = new QVBoxLayout;
        mainWidget->setLayout(mainLayout);

        //section: user script settings
        auto groupBox =  new QGroupBox;
        groupBox->setTitle(tr("User Scripts"));
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

        connect(browseFileButton, &QPushButton::clicked, this, [=](){
            auto dir = QFileDialog::getExistingDirectory(groupBox, tr("Select User Scripts Directory"), m_userScriptsDirLabel->text());
            if(!dir.isEmpty()) m_userScriptsDirLabel->setText(QDir::toNativeSeparators(dir));
        });
        connect(m_enableUserScriptsCheckBox, &QCheckBox::clicked, this, [=](bool checked){
           browseFileButton->setDisabled(!checked);
           m_userScriptsDirLabel->setDisabled(!checked);
        });
        m_enableUserScriptsCheckBox->setChecked(storedEnableUserScripts());
        m_userScriptsDirLabel->setText(storedUserScriptsDirectory());
        browseFileButton->setDisabled(!m_enableUserScriptsCheckBox->isChecked());
        m_userScriptsDirLabel->setDisabled(!m_enableUserScriptsCheckBox->isChecked());
        formLayout->addRow(new QLabel(tr("Note: Third-party scripts may contain malicious code. Make sure you trust them before adding.")));
        mainLayout->addWidget(groupBox);

        //section: script inspector
        auto inspector = new QGroupBox;
        inspector->setTitle("Script Inspector");
        auto inspectorLayout = new QHBoxLayout;
        inspector->setLayout(inspectorLayout);
        mainLayout->addWidget(inspector);
        m_scriptListWidget = new QTreeWidget;
        inspectorLayout->addWidget(m_scriptListWidget);
        m_scriptListWidget->setColumnCount(2);
        m_scriptListWidget->setHeaderLabels({tr("Script ID"), tr("Script Name")});
        m_scriptInfoWidget = new QTreeWidget;
        m_scriptInfoWidget->setColumnCount(2);
        m_scriptInfoWidget->setHeaderLabels({tr("Property"), tr("Value")});
        inspectorLayout->addWidget(m_scriptInfoWidget);
        updateScriptInspector();
        connect(m_scriptListWidget, &QTreeWidget::currentItemChanged, this, [=](QTreeWidgetItem *current, QTreeWidgetItem *previous){
            Q_UNUSED(previous);
            if(current && (current->flags() & Qt::ItemIsSelectable)) showScriptInfo(current->text(0), current->data(0, Qt::UserRole).toInt());
        });
        return m_widget = mainWidget;
    }
    bool ScriptSettingsPage::accept() {
        QDir dir(m_userScriptsDirLabel->text());
        if(!dir.exists()) return false;
        bool isModified = (m_enableUserScriptsCheckBox->isChecked() != storedEnableUserScripts()) || (m_userScriptsDirLabel->text() != storedUserScriptsDirectory());
        QJsonObject obj;
        obj.insert("enableUserScripts", m_enableUserScriptsCheckBox->isChecked());
        obj.insert("userScriptsDirectory", m_userScriptsDirLabel->text());
        Core::ILoader::instance()->settings()->insert("ScriptMgr", obj);
        if(isModified) {
            ScriptLoader::instance()->reloadEngine();
        }
        updateScriptInspector();
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
    void ScriptSettingsPage::updateScriptInspector() {
        m_scriptListWidget->clear();
        m_scriptInfoWidget->clear();
        createScriptList(tr("Built-in Scripts"), ScriptLoader::instance()->builtInScriptEntries());
        if(!ScriptLoader::instance()->scriptEntries().empty()) {
            createScriptList(tr("User Scripts"), ScriptLoader::instance()->scriptEntries());
        }
    }
    void ScriptSettingsPage::createScriptList(const QString &title, const QList<ScriptEntry> &entryList) {
        auto topItem = new QTreeWidgetItem;
        topItem->setText(0, title);
        topItem->setFlags(topItem->flags() & ~Qt::ItemIsSelectable);
        m_scriptListWidget->addTopLevelItem(topItem);
        for(const auto &entry: entryList) {
            auto item = new QTreeWidgetItem;
            item->setText(0, entry.id);
            item->setData(0, Qt::UserRole, entry.type);
            item->setText(1, ScriptLoader::instance()->getName(entry.id));
            topItem->addChild(item);
        }
        topItem->setExpanded(true);
    }
    void ScriptSettingsPage::showScriptInfo(const QString &id, int type) {
        m_scriptInfoWidget->clear();
        auto scriptInfo = ScriptLoader::instance()->getInfo(id).toVariant().toMap();

        m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("ID"), id}));

        m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Name"), JS_PROP_AS(scriptInfo, name, String)}));

        auto unknownPlaceholder = tr("<Unknown>");

        JS_PROP_OPTIONAL_IF(scriptInfo, vendor, String)
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Vendor"), vendorProp}));
        JS_PROP_OPTIONAL_ELSE
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Vendor"), unknownPlaceholder}));
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(scriptInfo, description, String)
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Description"), descriptionProp}));
        JS_PROP_OPTIONAL_ELSE
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Description"), unknownPlaceholder}));
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(scriptInfo, version, String)
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Version"), versionProp}));
        JS_PROP_OPTIONAL_ELSE
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Version"), unknownPlaceholder}));
        JS_PROP_OPTIONAL_ENDIF

        //TODO role

        if(type == ScriptEntry::Script) {
            JS_PROP_OPTIONAL_IF(scriptInfo, shortcut, String)
                m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Shortcut"),QKeySequence(shortcutProp).toString(QKeySequence::NativeText)}));
            JS_PROP_OPTIONAL_ENDIF
        } else {
            int i = 0;
            for(const auto &childScript: scriptInfo.value("children").toList()) {
                auto childTopItem = new QTreeWidgetItem({tr("Sub-action %1").arg(i++)});
                m_scriptInfoWidget->addTopLevelItem(childTopItem);
                childTopItem->addChild(new QTreeWidgetItem({tr("ID"), JS_PROP_AS(childScript.toMap(), id, String)}));
                childTopItem->addChild(new QTreeWidgetItem({tr("Name"), JS_PROP_AS(childScript.toMap(), name, String)}));
                JS_PROP_OPTIONAL_IF(childScript.toMap(), shortcut, String)
                    childTopItem->addChild(new QTreeWidgetItem({tr("Shortcut"), QKeySequence(shortcutProp).toString(QKeySequence::NativeText)}));
                JS_PROP_OPTIONAL_ENDIF
            }
        }

    }
} // Internal