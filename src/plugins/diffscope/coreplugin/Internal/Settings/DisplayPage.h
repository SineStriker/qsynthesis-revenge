#ifndef CHORUSKIT_DISPLAYPAGE_H
#define CHORUSKIT_DISPLAYPAGE_H

#include <CoreApi/ISettingPage.h>

namespace Core {

    namespace Internal {

        class DisplayPage : public ISettingPage {
            Q_OBJECT
        public:
            explicit DisplayPage(QObject *parent = nullptr);
            ~DisplayPage();

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

#endif // CHORUSKIT_DISPLAYPAGE_H
