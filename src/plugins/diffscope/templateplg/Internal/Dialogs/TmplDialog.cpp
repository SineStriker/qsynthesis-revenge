#include "TmplDialog.h"


namespace TemplatePlg {
    namespace Internal {
        TmplDialog::TmplDialog(QWidget *parent, QString title, QString message, QString okButtonText,
                               QString cancelButtonText)
            : QDialog(parent) {
            QLabel *label = new QLabel(message);
            QPushButton *okButton = new QPushButton(okButtonText);
            QPushButton *cancelButton = new QPushButton(cancelButtonText);

            connect(okButton, &QPushButton::clicked, this, &TmplDialog::accept);
            connect(cancelButton, &QPushButton::clicked, this, &TmplDialog::reject);

            QVBoxLayout *layout = new QVBoxLayout;
            QHBoxLayout *buttonLayout = new QHBoxLayout;

            buttonLayout->addStretch();
            buttonLayout->addWidget(okButton);
            buttonLayout->addWidget(cancelButton);

            layout->addWidget(label);
            layout->addLayout(buttonLayout);

            setLayout(layout);
            setWindowTitle(title);
        }
    }
}