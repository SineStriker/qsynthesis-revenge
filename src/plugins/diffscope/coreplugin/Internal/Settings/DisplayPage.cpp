#include "DisplayPage.h"

#include <QMDecorator.h>

#include <QLabel>

namespace Core {

    namespace Internal {

        DisplayPage::DisplayPage(QObject *parent) : ISettingPage("core_Display", parent) {
            m_widget = nullptr;

            qIDec->installLocale(this, {{}}, _LOC(DisplayPage, this));
        }

        DisplayPage::~DisplayPage() {
        }

        void DisplayPage::reloadStrings() {
            setTitle(tr("Display"));
        }

        QString DisplayPage::sortKeyword() const {
            return "Display";
        }

        bool DisplayPage::matches(const QString &word) const {
            return ISettingPage::matches(word);
        }

        QWidget *DisplayPage::widget() {
            if (!m_widget) {
                auto label = new QLabel();
                label->setText("Display");
                m_widget = label;
            }
            return m_widget;
        }

        bool DisplayPage::accept() {
            return true;
        }

        void DisplayPage::finish() {
            if (m_widget) {
                m_widget->deleteLater();
                m_widget = nullptr;
            }
        }

    }

}