#include "AppearanceTopPage.h"

#include <QMDecoratorV2.h>

#include <QLabel>

namespace Core {

    namespace Internal {

        AppearanceTopPage::AppearanceTopPage(QObject *parent) : ISettingPage("core.AppearanceTop", parent) {
            m_widget = nullptr;
            setTitle([]() { return tr("Appearance"); });
            setDescription([]() { return tr("Application appearance."); });
        }

        AppearanceTopPage::~AppearanceTopPage() {
        }

        QString AppearanceTopPage::sortKeyword() const {
            return "Appearance";
        }

        bool AppearanceTopPage::matches(const QString &word) const {
            return ISettingPage::matches(word);
        }

        QWidget *AppearanceTopPage::widget() {
            return nullptr;

            if (!m_widget) {
                auto label = new QLabel();
                label->setText("Appearance Top");
                m_widget = label;
            }
            return m_widget;
        }

        bool AppearanceTopPage::accept() {
            return true;
        }

        void AppearanceTopPage::finish() {
            if (m_widget) {
                m_widget->deleteLater();
                m_widget = nullptr;
            }
        }

    }

}