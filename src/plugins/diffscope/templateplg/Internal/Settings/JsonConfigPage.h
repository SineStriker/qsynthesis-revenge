#ifndef CHORUSKIT_JSONCONFIGPAGE_H
#define CHORUSKIT_JSONCONFIGPAGE_H

#include <CoreApi/ISettingPage.h>

namespace TemplatePlg {
    using namespace Core;
    class JsonConfigPage : public ISettingPage {
        Q_OBJECT
    public:
        explicit JsonConfigPage(QObject *parent = nullptr);
        ~JsonConfigPage();

        void reloadStrings();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QWidget *m_widget;
    };



}

#endif // CHORUSKIT_JSONCONFIGPAGE_H
