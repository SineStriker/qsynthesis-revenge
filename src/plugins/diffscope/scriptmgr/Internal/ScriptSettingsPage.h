#ifndef CHORUSKIT_SCRIPTSETTINGSPAGE_H
#define CHORUSKIT_SCRIPTSETTINGSPAGE_H

#include <CoreApi/ISettingPage.h>
#include "ScriptLoader.h"

class QCheckBox;
class QLineEdit;
class QListWidget;
class QTreeWidget;
class QTreeWidgetItem;

namespace ScriptMgr::Internal {

    class ScriptSettingsPage: public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit ScriptSettingsPage(QObject *parent = nullptr);
        ~ScriptSettingsPage();
        QString sortKeyword() const override;
        bool matches(const QString &word) const override;
        QWidget *widget() override;
        bool accept() override;
        void finish() override;

        static bool storedEnableUserScripts();
        static QString storedUserScriptsDirectory();
    private:
        void updateScriptInspector();
        void createScriptList(const QString &title, const QList<ScriptEntry> &entryList);
        void showScriptInfo(const QString &id, int type);

        QWidget *m_widget;
        QCheckBox *m_enableUserScriptsCheckBox;
        QLineEdit *m_userScriptsDirLabel;
        QTreeWidget *m_scriptListWidget;
        QTreeWidget *m_scriptInfoWidget;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTSETTINGSPAGE_H
