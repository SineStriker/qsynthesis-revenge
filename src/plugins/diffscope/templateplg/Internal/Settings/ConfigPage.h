#ifndef CHORUSKIT_CONFIGPAGE_H
#define CHORUSKIT_CONFIGPAGE_H

#include <CoreApi/ISettingPage.h>

namespace TemplatePlg {
    namespace Internal {
        using namespace Core;
        class ConfigPage : public ISettingPage {
            Q_OBJECT
        public:
            explicit ConfigPage(QObject *parent = nullptr);
            ~ConfigPage();

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

}

#endif // CHORUSKIT_CONFIGPAGE_H
