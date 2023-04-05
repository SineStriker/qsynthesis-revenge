//
// Created by Crs_1 on 2023/4/4.
//

#ifndef CHORUSKIT_TESTDIALOGPAGE_H
#define CHORUSKIT_TESTDIALOGPAGE_H

#include "CoreApi/IDialogPage.h"
#include <QLabel>
#include <QLineEdit>

namespace Core {
    class TestDialogPage: public IDialogPage { //test
        Q_OBJECT
    public:
        ~TestDialogPage();
        QString id() const override;
        QString title() const override;
        QWidget * widget() override;
        QString getEditText() const;
        void reloadStrings() override;
    public slots:
        void load() override;
        void accept() override;
        void finish() override;
        void setLabelText(const QString &text);
    private:
        QWidget *m_widget = nullptr;
        QLineEdit *m_lineEdit = nullptr;
        QLabel *m_label = nullptr;
        QString m_staged_text;
    };
}



#endif // CHORUSKIT_TESTDIALOGPAGE_H
