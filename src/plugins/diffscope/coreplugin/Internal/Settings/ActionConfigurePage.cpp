#include "ActionConfigurePage.h"

#include <QMDecoratorV2.h>

#include <QLabel>

namespace Core::Internal {

    ActionConfigurePage::ActionConfigurePage(QObject *parent) : ISettingPage("core.ActionConfigure", parent) {
        m_widget = nullptr;
        setTitle([]() { return tr("Actions and Toolbars"); });
        setDescription([]() { return tr("Actions and Toolbars"); });
    }

    ActionConfigurePage::~ActionConfigurePage() {
    }

    QString ActionConfigurePage::sortKeyword() const {
        return "Actions and Toolbars";
    }

    bool ActionConfigurePage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *ActionConfigurePage::widget() {
        if (!m_widget) {
            auto label = new QLabel();
            label->setText("Action Configure");
            m_widget = label;
        }
        return m_widget;
    }

    bool ActionConfigurePage::accept() {
        return true;
    }

    void ActionConfigurePage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}
