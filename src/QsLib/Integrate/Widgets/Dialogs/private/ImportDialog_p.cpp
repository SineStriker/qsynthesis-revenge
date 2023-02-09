#include "ImportDialog_p.h"

#include <QScrollBar>

#include "CDecorator.h"

static QString convertString(QTextCodec *codec, const QByteArray &data) {
    QTextCodec::ConverterState state;
    QString res = codec->toUnicode(data.constData(), data.size(), &state);
    if (state.invalidChars > 0) {
        res = ImportDialog::tr("(Decoding failure)");
    }
    return res;
}

enum Role {
    NameRole = Qt::UserRole + 1,
    ContentRole,
};

ImportDialogPrivate::ImportDialogPrivate() {
}

ImportDialogPrivate::~ImportDialogPrivate() {
}

void ImportDialogPrivate::init() {
    Q_Q(ImportDialog);

    firstShow = true;
    maxInitHeight = 0;
    codec = nullptr;

    q->setWindowFlags(q->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Tracks
    boxGroup = new QButtonGroup(q);
    boxGroup->setExclusive(false);

    boxesLayout = new QVBoxLayout();
    boxesLayout->setSpacing(0);
    boxesLayout->setMargin(0);

    boxesWidget = new QWidget();
    boxesWidget->setAttribute(Qt::WA_StyledBackground);
    boxesWidget->setObjectName("tracks-widget");
    boxesWidget->setLayout(boxesLayout);

    tracksScroll = new LinearScrollArea(Qt::Vertical);
    // tracksScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tracksScroll->setObjectName("tracks-scroll");
    tracksScroll->setWidget(boxesWidget);

    // Codecs
    codecWidget = new QSplitter(Qt::Horizontal);
    codecWidget->setObjectName("codec-widget");

    codecListWidget = new QListWidget();
    codecListWidget->setObjectName("codec-list-widget");
    for (const auto &s : SupportEncodings) {
        codecListWidget->addItem(s);
    }

    nameListWidget = new QListWidget();
    nameListWidget->setObjectName("name-list-widget");

    lyricsWidget = new QPlainTextEdit();
    lyricsWidget->setObjectName("lyrics-widget");

    codecWidget->addWidget(codecListWidget);
    codecWidget->addWidget(nameListWidget);
    codecWidget->addWidget(lyricsWidget);

    // Tab widget
    tabWidget = new QTabWidget();
    tabWidget->setObjectName("tab-widget");
    tabWidget->addTab(tracksScroll, tr("Select tracks"));
    tabWidget->addTab(codecWidget, tr("Select encoding"));

    // Bottom buttons
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

    // Main
    layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(tabWidget);
    layout->addLayout(buttonsLayout);

    q->setLayout(layout);

    connect(btnOK, &QPushButton::clicked, this, &ImportDialogPrivate::_q_okButtonClicked);
    connect(okAction, &QAction::triggered, this, &ImportDialogPrivate::_q_okButtonClicked);
    connect(btnCancel, &QPushButton::clicked, this, &ImportDialogPrivate::_q_cancelButtonClicked);
    connect(tracksScroll->verticalScrollBar(), &QScrollBar::rangeChanged, this,
            &ImportDialogPrivate::_q_scrollRangeChanged);

    connect(codecListWidget, &QListWidget::currentItemChanged, this,
            &ImportDialogPrivate::_q_currentCodecChanged);
    connect(nameListWidget, &QListWidget::currentItemChanged, this,
            &ImportDialogPrivate::_q_currentNameChanged);

    qIDec->installLocale(q, {"QsIntegrate"}, _LOC(ImportDialog, q));
    qIDec->installTheme(q, {"ImportDialog"});

    codecListWidget->setCurrentRow(0);
}

void ImportDialogPrivate::updateEncoding() {
    auto cur = codecListWidget->currentItem();
    if (!cur) {
        return;
    }
    auto codec = QTextCodec::codecForName(cur->text().toLatin1());
    for (int i = 0; i < nameListWidget->count(); ++i) {
        const auto &fmt = opt.tracks.at(i).format;
        auto item = nameListWidget->item(i);
        item->setText(fmt.arg(convertString(codec, item->data(NameRole).toByteArray())));
        if (nameListWidget->currentRow() == i) {
            lyricsWidget->setPlainText(convertString(codec, item->data(ContentRole).toByteArray()));
        }
    }
}

void ImportDialogPrivate::updateNameList() {
    nameListWidget->blockSignals(true);
    nameListWidget->clear();
    for (const auto &info : qAsConst(opt.tracks)) {
        auto item = new QListWidgetItem();
        item->setData(NameRole, info.title);
        item->setData(ContentRole, info.lyrics);
        nameListWidget->addItem(item);
    }
    if (nameListWidget->count() > 0) {
        nameListWidget->setCurrentRow(0);
    }
    nameListWidget->blockSignals(false);
    updateEncoding();
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

void ImportDialogPrivate::_q_currentCodecChanged(QListWidgetItem *cur, QListWidgetItem *prev) {
    Q_UNUSED(cur);
    Q_UNUSED(prev);
    updateEncoding();
}

void ImportDialogPrivate::_q_currentNameChanged(QListWidgetItem *cur, QListWidgetItem *prev) {
    Q_UNUSED(prev);
    auto codecItem = codecListWidget->currentItem();
    if (!codecItem) {
        lyricsWidget->clear();
        return;
    }
    auto codec = QTextCodec::codecForName(codecItem->text().toLatin1());
    lyricsWidget->setPlainText(convertString(codec, cur->data(ContentRole).toByteArray()));
}
