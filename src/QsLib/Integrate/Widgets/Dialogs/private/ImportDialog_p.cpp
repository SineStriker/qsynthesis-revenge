#include "ImportDialog_p.h"

#include <QScrollBar>

#include "CDecorator.h"

ImportDialogPrivate::ImportDialogPrivate() {
}

ImportDialogPrivate::~ImportDialogPrivate() {
}

void ImportDialogPrivate::init() {
    Q_Q(ImportDialog);

    firstShow = true;
    maxInitHeight = 0;

    q->setWindowFlags(q->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    codec = nullptr;
    boxGroup = new QButtonGroup(q);
    boxGroup->setExclusive(false);

    // Add label and buttons
    lbCaption = new QLabel();
    lbCaption->setObjectName("caption-label");
    lbCaption->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    btnCancel = new CTabButton();
    btnCancel->setProperty("type", "done-button");
    btnCancel->setObjectName("cancel-button");

    btnOK = new CTabButton();
    btnOK->setProperty("type", "done-button");
    btnOK->setObjectName("ok-button");
    btnOK->setFocus();
    btnOK->setDefault(true);

    okAction = new QAction(btnOK);
    okAction->setShortcuts({QKeySequence(Qt::Key_Enter), QKeySequence(Qt::Key_Return)});
    btnOK->addAction(okAction);

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(0);
    buttonsLayout->setMargin(0);

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(btnOK);
    buttonsLayout->addWidget(btnCancel);

    boxesLayout = new QVBoxLayout();
    boxesLayout->setSpacing(0);
    boxesLayout->setMargin(0);

    boxesWidget = new QWidget();
    boxesWidget->setAttribute(Qt::WA_StyledBackground);
    boxesWidget->setObjectName("boxes-widget");
    boxesWidget->setLayout(boxesLayout);

    boxesScroll = new LinearScrollArea(Qt::Vertical);
    boxesScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    boxesScroll->setObjectName("boxes-scroll");
    boxesScroll->setWidget(boxesWidget);

    layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(lbCaption);
    layout->addWidget(boxesScroll);
    layout->addLayout(buttonsLayout);

    q->setLayout(layout);

    connect(btnOK, &QPushButton::clicked, this, &ImportDialogPrivate::_q_okButtonClicked);
    connect(okAction, &QAction::triggered, this, &ImportDialogPrivate::_q_okButtonClicked);
    connect(btnCancel, &QPushButton::clicked, this, &ImportDialogPrivate::_q_cancelButtonClicked);
    connect(boxesScroll->verticalScrollBar(), &QScrollBar::rangeChanged, this,
            &ImportDialogPrivate::_q_scrollRangeChanged);

    qIDec->installLocale(q, {"QsIntegrate"}, _LOC(ImportDialog, q));
    qIDec->installTheme(q, {"ImportDialog"});
}

void ImportDialogPrivate::_q_boxToggled(bool checked) {
    auto btn = qobject_cast<QAbstractButton *>(sender());
    if (checked) {
        // Add
        {
            auto it = queue.insert(queue.end(), btn);
            queueMap.insert(btn, it);
        }

        // Remove earliest
        if (queue.size() > (size_t) opt.maxTracks) {
            auto it = queue.begin();
            auto btn2 = *it;
            queueMap.remove(btn2);
            queue.erase(it);
            btn2->setChecked(false);
        }
    } else {
        auto it = queueMap.find(btn);
        if (it != queueMap.end()) {
            queue.erase(it.value());
            queueMap.erase(it);
        }
    }
}

void ImportDialogPrivate::_q_okButtonClicked() {
    Q_Q(ImportDialog);

    auto bl = boxesLayout;
    for (int i = 0; i < bl->count(); ++i) {
        if (qobject_cast<QAbstractButton *>(bl->itemAt(i)->widget())->isChecked()) {
            trackIndexs.append(i);
        }
    }
    q->accept();
}

void ImportDialogPrivate::_q_cancelButtonClicked() {
    Q_Q(ImportDialog);
    q->reject();
}

void ImportDialogPrivate::_q_scrollRangeChanged(int min, int max) {
    Q_Q(ImportDialog);
    if (firstShow) {
        firstShow = false;
        q->resize(q->width(), qMax(qMin(maxInitHeight, q->height() + max - min), q->height()));
    }
}
