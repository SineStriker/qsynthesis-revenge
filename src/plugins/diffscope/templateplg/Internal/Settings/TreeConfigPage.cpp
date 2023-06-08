#include "TreeConfigPage.h"
#include <QMDecoratorV2.h>

namespace TemplatePlg::Internal {

    TreeConfigPage::TreeConfigPage(QObject *parent) : ISettingPage("core.JsonConfig", parent) {
        m_widget = nullptr;
        setTitle([]() { return tr("JsonConfig"); });
        setDescription([]() { return tr("JsonConfig"); });

        qIDec->installLocale(this, _LOC(TreeConfigPage, this));
    }

    TreeConfigPage::~TreeConfigPage() {
    }

    void TreeConfigPage::reloadStrings() {
    }

    QString TreeConfigPage::sortKeyword() const {
        return "JsonConfig";
    }

    bool TreeConfigPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *TreeConfigPage::widget() {
        if (!m_widget) {
            m_widget = TreeConfigWidget::instance("core.tmpl")->configWidget();
        }
        return m_widget;
    }

    bool TreeConfigPage::accept() {
        return true;
    }

    void TreeConfigPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}