#include "TreeConfigPage.h"
#include <QMDecoratorV2.h>

namespace TemplatePlg {
    namespace Internal {
        TreeConfigPage::TreeConfigPage(QObject *parent) : ISettingPage("core.JsonConfig", parent) {
            m_widget = nullptr;
            qIDec->installLocale(this, _LOC(TreeConfigPage, this));
        }

        TreeConfigPage::~TreeConfigPage() {
        }

        void TreeConfigPage::reloadStrings() {
            setTitle(tr("JsonConfig"));
            setDescription(tr("JsonConfig"));
        }

        QString TreeConfigPage::sortKeyword() const {
            return "JsonConfig";
        }

        bool TreeConfigPage::matches(const QString &word) const {
            return ISettingPage::matches(word);
        }

        QWidget *TreeConfigPage::widget() {
            if (!m_widget) {
                m_widget = TreeConfigWidget::Instance()->configWidget();
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
}