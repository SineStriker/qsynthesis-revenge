#ifndef CHORUSKIT_APPEARANCETOPPAGE_H
#define CHORUSKIT_APPEARANCETOPPAGE_H

#include <CoreApi/ISettingPage.h>

namespace Core {

    namespace Internal {

        class AppearanceTopPage : public ISettingPage {
            Q_OBJECT
        public:
            explicit AppearanceTopPage(QObject *parent = nullptr);
            ~AppearanceTopPage();

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

#endif // CHORUSKIT_APPEARANCETOPPAGE_H
