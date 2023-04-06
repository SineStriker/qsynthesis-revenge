//
// Created by Crs_1 on 2023/4/6.
//

#ifndef CHORUSKIT_GENERALOPTIONPAGE_H
#define CHORUSKIT_GENERALOPTIONPAGE_H

#include "Preferences/IOptionsPage.h"
#include <QLabel>
#include <QLineEdit>
namespace Core {

    class GeneralOptionsPage : public IOptionsPage {
        Q_OBJECT
    public:
        ~GeneralOptionsPage();
        QString id() const override;
        QString title() const override;
        QString description() const override;
        QWidget * widget() override;
        void reloadStrings() override;
        bool match(const QString &pattern) const override;
    public slots:
        void load() override;
        void accept() override;
        void finish() override;
    private:
        QWidget *m_widget = nullptr;
        QLabel *m_label = nullptr;
        QLineEdit *m_lineEdit = nullptr;
    };

} // Core

#endif // CHORUSKIT_GENERALOPTIONPAGE_H
