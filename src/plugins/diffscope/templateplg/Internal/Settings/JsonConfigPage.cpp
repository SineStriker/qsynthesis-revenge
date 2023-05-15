#include "../Widgets/JsonConfigWidget.h"

#include "JsonConfigPage.h"
#include <QMDecoratorV2.h>

namespace TemplatePlg {

    JsonConfigPage::JsonConfigPage(QObject *parent) : ISettingPage("core.JsonConfig", parent) {
        m_widget = nullptr;

        qIDec->installLocale(this, _LOC(JsonConfigPage, this));
    }

    JsonConfigPage::~JsonConfigPage() {
    }

    void JsonConfigPage::reloadStrings() {
        setTitle(tr("JsonConfig"));
        setDescription(tr("JsonConfig"));
    }

    QString JsonConfigPage::sortKeyword() const {
        return "JsonConfig";
    }

    bool JsonConfigPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *JsonConfigPage::widget() {
        if (!m_widget) {
            m_widget = new JsonConfigWidget();
        }
        return m_widget;
    }

    bool JsonConfigPage::accept() {
        return true;
    }

    void JsonConfigPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }



}