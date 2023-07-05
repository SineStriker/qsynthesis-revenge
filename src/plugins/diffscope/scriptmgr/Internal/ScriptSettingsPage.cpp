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
#include <QKeySequenceEdit>

#include "JsMacros.h"
#include "ScriptLoader.h"

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
        inspector->setTitle(tr("Script Inspector"));
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
        connect(m_scriptInfoWidget, &QTreeWidget::itemActivated, this, [=](QTreeWidgetItem *item, int index){
            if(!item) return;
            if(item->data(1, IdRole).isNull()) return;
            auto id = item->data(1, IdRole).toString();
            auto defaultShortcut = item->data(1, DefaultShortcutRole).value<QKeySequence>();
            auto customShortcut = item->data(1, CustomShortcutRole).value<QKeySequence>();

            QDialog dlg;
            dlg.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            dlg.setWindowTitle(tr("Custom Shortcut"));
            auto dlgLayout = new QVBoxLayout;
            auto enableCustomShortcutCheckBox = new QCheckBox(tr("Enable custom shortcut"));
            auto shortcutEdit = new QKeySequenceEdit;
            auto okButton = new QPushButton(tr("OK"));
            dlgLayout->addWidget(enableCustomShortcutCheckBox);
            dlgLayout->addWidget(shortcutEdit);
            dlgLayout->addSpacing(8);
            dlgLayout->addWidget(okButton);
            dlg.setLayout(dlgLayout);

            if(ScriptLoader::instance()->m_cachedCustomShortcuts.contains(id) || m_unsavedCustomShortcuts.contains(id)) {
                enableCustomShortcutCheckBox->setChecked(true);
                shortcutEdit->setKeySequence(customShortcut);
            } else {
                shortcutEdit->setKeySequence(defaultShortcut);
                shortcutEdit->setDisabled(true);
            }

            connect(okButton, &QPushButton::clicked, &dlg, &QDialog::accept);
            connect(enableCustomShortcutCheckBox, &QCheckBox::clicked, shortcutEdit, [=](bool checked){
                if(checked) {
                    shortcutEdit->setDisabled(false);
                } else {
                    shortcutEdit->setDisabled(true);
                    shortcutEdit->setKeySequence(defaultShortcut);
                }
            });
            connect(&dlg, &QDialog::accepted, [=]{
                if(enableCustomShortcutCheckBox->isChecked() && !shortcutEdit->keySequence().isEmpty()) {
                    m_unsavedCustomShortcuts[id] = shortcutEdit->keySequence();
                    setShortcutItem(item, defaultShortcut, shortcutEdit->keySequence());
                } else {
                    m_unsavedCustomShortcuts[id] = {};
                    setShortcutItem(item, defaultShortcut, {});
                }
            });
            dlg.exec();
        });
        return m_widget = mainWidget;
    }
    bool ScriptSettingsPage::accept() {
        if(!m_widget) return true;
        QDir dir(m_userScriptsDirLabel->text());
        if(!dir.exists()) return false;
        bool isModified = (m_enableUserScriptsCheckBox->isChecked() != storedEnableUserScripts()) ||
                          (m_userScriptsDirLabel->text() != storedUserScriptsDirectory()) ||
                          (!m_unsavedCustomShortcuts.isEmpty());
        QJsonObject obj;
        obj.insert("enableUserScripts", m_enableUserScriptsCheckBox->isChecked());
        obj.insert("userScriptsDirectory", m_userScriptsDirLabel->text());

        auto customShortcutsObject = Core::ILoader::instance()->settings()->value("ScriptMgr").toObject().value("customShortcuts").toObject();
        for(const auto &key: m_unsavedCustomShortcuts.keys()) {
            auto shortcut = m_unsavedCustomShortcuts.value(key);
            if(shortcut.isEmpty()) {
                customShortcutsObject.remove(key);
            } else {
                customShortcutsObject.insert(key, shortcut.toString());
            }
        }
        obj.insert("customShortcuts", customShortcutsObject);

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
        if(directory.isString()) {
            return directory.toString();
        } else {
            auto defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ChorusKit/DiffScope/User Scripts";
            if(!QDir(defaultPath).exists()) {
                QDir().mkpath(defaultPath);
            }
            return QDir::toNativeSeparators(defaultPath);
        }
    }

    QMap<QString, QKeySequence> ScriptSettingsPage::storedCustomShortcuts() {
        auto shortcutsJsonObject = Core::ILoader::instance()->settings()->value("ScriptMgr").toObject().value("customShortcuts").toObject();
        QMap<QString, QKeySequence> customShortcuts;
        for(const auto &key: shortcutsJsonObject.keys()) {
            customShortcuts.insert(key, QKeySequence(shortcutsJsonObject.value(key).toString()));
        }
        return customShortcuts;
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

        JS_PROP_OPTIONAL_IF(scriptInfo, requiredVersion, String)
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Required Version"), requiredVersionProp}));
        JS_PROP_OPTIONAL_ELSE
            m_scriptInfoWidget->addTopLevelItem(new QTreeWidgetItem({tr("Required Version"), unknownPlaceholder}));
        JS_PROP_OPTIONAL_ENDIF

        //TODO role

        if(type == ScriptEntry::Script) {
            auto *shortcutItem = new QTreeWidgetItem({tr("Shortcut"), ""});
            QKeySequence customShortcut, defaultShortcut;
            if(ScriptLoader::instance()->m_cachedCustomShortcuts.contains(id)) {
                customShortcut = ScriptLoader::instance()->m_cachedCustomShortcuts.value(id);
            }
            JS_PROP_OPTIONAL_IF(scriptInfo, shortcut, String)
                defaultShortcut = QKeySequence(shortcutProp);
            JS_PROP_OPTIONAL_ENDIF
            setShortcutItem(shortcutItem, defaultShortcut, customShortcut);
            shortcutItem->setData(1, IdRole, id);
            m_scriptInfoWidget->addTopLevelItem(shortcutItem);
        } else {
            int i = 0;
            for(const auto &childScript: scriptInfo.value("children").toList()) {
                auto childTopItem = new QTreeWidgetItem({tr("Sub-action %1").arg(i++)});
                m_scriptInfoWidget->addTopLevelItem(childTopItem);
                auto childId = JS_PROP_AS(childScript.toMap(), id, String);
                childTopItem->addChild(new QTreeWidgetItem({tr("ID"), childId}));
                childTopItem->addChild(new QTreeWidgetItem({tr("Name"), JS_PROP_AS(childScript.toMap(), name, String)}));
                auto *shortcutItem = new QTreeWidgetItem({tr("Shortcut"), ""});
                QKeySequence customShortcut, defaultShortcut;
                if(ScriptLoader::instance()->m_cachedCustomShortcuts.contains(id + "." + childId)) {
                    customShortcut = ScriptLoader::instance()->m_cachedCustomShortcuts.value(id + "." + childId);
                }
                JS_PROP_OPTIONAL_IF(childScript.toMap(), shortcut, String)
                    defaultShortcut = QKeySequence(shortcutProp);
                JS_PROP_OPTIONAL_ENDIF
                setShortcutItem(shortcutItem, defaultShortcut, customShortcut);
                shortcutItem->setData(1, IdRole, id + "." + childId);
                childTopItem->addChild(shortcutItem);
            }
        }

    }
    void ScriptSettingsPage::setShortcutItem(QTreeWidgetItem *item, const QKeySequence &defaultShortcut, const QKeySequence &customShortcut) {
        item->setData(1, CustomShortcutRole, customShortcut);
        item->setData(1, DefaultShortcutRole, defaultShortcut);
        if(!customShortcut.isEmpty()) {
            item->setText(1, customShortcut.toString(QKeySequence::NativeText));
            auto font = item->font(1);
            font.setItalic(true);
            item->setFont(1, font);
        } else if(!defaultShortcut.isEmpty()) {
            auto font = item->font(1);
            font.setItalic(false);
            item->setFont(1, font);
            item->setText(1, defaultShortcut.toString(QKeySequence::NativeText));
        } else {
            auto font = item->font(1);
            font.setItalic(false);
            item->setFont(1, font);
            item->setText(1, tr("<None>"));
        }
    }
} // Internal