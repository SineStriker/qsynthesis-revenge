#ifndef CHORUSKIT_SCRIPTSETTINGSPAGE_H
#define CHORUSKIT_SCRIPTSETTINGSPAGE_H

#include <CoreApi/ISettingPage.h>

class QCheckBox;
class QLineEdit;

namespace ScriptMgr::Internal {

    class ScriptSettingsPage: public Core::ISettingPage {
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
        QWidget *m_widget;
        QCheckBox *m_enableUserScriptsCheckBox;
        QLineEdit *m_userScriptsDirLabel;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTSETTINGSPAGE_H
