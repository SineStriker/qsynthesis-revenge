#include "ImportDialog_p.h"

#include "CDecorator.h"

ImportDialogPrivate::ImportDialogPrivate() {
}

ImportDialogPrivate::~ImportDialogPrivate() {
}

void ImportDialogPrivate::init() {
    Q_Q(ImportDialog);

    q->setWindowFlags(q->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    codec = nullptr;
    boxGroup = new QButtonGroup(q);
    boxGroup->setExclusive(false);

    // Add label and buttons
    lbCaption = new QLabel();
    lbCaption->setObjectName("caption-label");
    lbCaption->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    btnCancel = new CPushButton();
    btnCancel->setObjectName("cancel-button");

    btnOK = new CPushButton();
    btnOK->setObjectName("ok-button");
    btnOK->setFocus();
    btnOK->setDefault(true);

    okAction = new QAction(q);
    okAction->setShortcuts({QKeySequence(Qt::Key_Enter), QKeySequence(Qt::Key_Return)});

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(0);
    buttonsLayout->setMargin(0);

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(btnOK);
    buttonsLayout->addWidget(btnCancel);

    boxesLayout = new QVBoxLayout();
    boxesLayout->setSpacing(0);
    boxesLayout->setMargin(0);

    layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(lbCaption);
    layout->addLayout(boxesLayout);
    layout->addLayout(buttonsLayout);

    q->setLayout(layout);

    q->connect(btnOK, &QPushButton::clicked, q, &ImportDialog::_q_okButtonClicked);
    q->connect(okAction, &QAction::triggered, q, &ImportDialog::_q_okButtonClicked);
    q->connect(btnCancel, &QPushButton::clicked, q, &ImportDialog::_q_cancelButtonClicked);

    qIDec->installLocale(q, {"QsIntegrate"}, _LOC(ImportDialog, q));
    qIDec->installTheme(q, {"ImportDialog"});
}
