#ifndef CHORUSKIT_TMPLDIALOG_H
#define CHORUSKIT_TMPLDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace TemplatePlg {
    namespace Internal {

        class TmplDialog : public QDialog {
            Q_OBJECT
        public:
            TmplDialog(QWidget *parent, QString title, QString message, QString okButtonText, QString cancelButtonText);

        private:
            QLabel *m_label;
            QPushButton *m_okButton;
            QPushButton *m_cancelButton;
        };
    }
}

#endif // CHORUSKIT_TMPLDIALOG_H
