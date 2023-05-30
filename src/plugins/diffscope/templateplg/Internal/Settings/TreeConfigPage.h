#ifndef CHORUSKIT_TREECONFIGPAGE_H
#define CHORUSKIT_TREECONFIGPAGE_H

#include "../Widgets/TreeConfigWidget.h"
#include <CoreApi/ISettingPage.h>

namespace TemplatePlg::Internal {
        using namespace Core;
        class TreeConfigPage : public ISettingPage {
            Q_OBJECT
        public:
            explicit TreeConfigPage(QObject *parent = nullptr);
            ~TreeConfigPage() override;

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

#endif // CHORUSKIT_TREECONFIGPAGE_H
