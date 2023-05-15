#include "../Widgets/ConfigWidget.h"

#include "ConfigPage.h"
#include <QMDecoratorV2.h>

namespace TemplatePlg {

    ConfigPage::ConfigPage(QObject *parent) : ISettingPage("core.Config", parent) {
        m_widget = nullptr;

        qIDec->installLocale(this, _LOC(ConfigPage, this));
    }

    ConfigPage::~ConfigPage() {
    }

    void ConfigPage::reloadStrings() {
        setTitle(tr("Config"));
        setDescription(tr("Config"));
    }

    QString ConfigPage::sortKeyword() const {
        return "Config";
    }

    bool ConfigPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *ConfigPage::widget() {
        if (!m_widget) {
            m_widget = new ConfigWidget();
        }
        return m_widget;
    }

    bool ConfigPage::accept() {
        return true;
    }

    void ConfigPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }



}