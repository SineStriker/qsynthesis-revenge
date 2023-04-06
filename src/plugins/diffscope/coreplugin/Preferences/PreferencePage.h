//
// Created by Crs_1 on 2023/4/6.
//

#ifndef CHORUSKIT_PREFERENCEPAGE_H
#define CHORUSKIT_PREFERENCEPAGE_H



#include "CoreApi/IDialogPage.h"
#include "IOptionsPage.h"
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTreeWidget>
namespace Core {
    class PreferencePage: public IDialogPage {
        Q_OBJECT
    public:
        ~PreferencePage();
        QString id() const override;
        QString title() const override;
        void reloadStrings() override;
        QWidget *widget() override;
    public slots:
        void load() override;
        void accept() override;
        void finish() override;
        void setPage(IOptionsPage *page);
    private:
        friend class PreferenceRegistryPrivate;
        friend class PreferenceRegistry;
        QWidget *m_widget = nullptr;
        QLineEdit *m_filterEdit = nullptr;
        QTreeWidget *m_treeWidget = nullptr;
        QLabel *m_titleLabel = nullptr;
        QLabel *m_descriptionLabel = nullptr;
        QStackedWidget *m_stackedWidget = nullptr;
    };
}



#endif // CHORUSKIT_PREFERENCEPAGE_H
