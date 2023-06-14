#include "TextWidget.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

TextWidget::TextWidget(QWidget *parent) : QWidget(parent), g2p(new IKg2p::ZhG2p()), g2p_jp(new IKg2p::JpG2p()) {
    wordsText = new QLineEdit();
    wordsText->setPlaceholderText("Enter mandarin here...");

    pasteButton = new QPushButton();
    pasteButton->setProperty("type", "user");
    pasteButton->setObjectName("paste-button");
    pasteButton->setIcon(QIcon(":/res/clipboard.svg"));

    lineLayout = new QHBoxLayout();
    lineLayout->setMargin(0);
    lineLayout->addWidget(wordsText);
    lineLayout->addWidget(pasteButton);

    contentText = new QPlainTextEdit();

    replaceButton = new QPushButton("Replace");
    replaceButton->setProperty("type", "user");

    replaceAction = new QAction();
    replaceAction->setShortcuts({QKeySequence(Qt::Key_Enter), QKeySequence(Qt::Key_Return)});
    replaceButton->addAction(replaceAction);

    appendButton = new QPushButton("Append");
    appendButton->setProperty("type", "user");

    languageCombo = new QComboBox();
    languageCombo->addItems({"pinyin", "romaji"});

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setMargin(0);
    buttonsLayout->addWidget(replaceButton);
    buttonsLayout->addWidget(appendButton);
    buttonsLayout->addWidget(languageCombo);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(lineLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(contentText);

    setLayout(mainLayout);

    connect(pasteButton, &QPushButton::clicked, this, &TextWidget::_q_pasteButtonClicked);
    connect(replaceButton, &QPushButton::clicked, this, &TextWidget::_q_replaceButtonClicked);
    connect(appendButton, &QPushButton::clicked, this, &TextWidget::_q_appendButtonClicked);

    connect(replaceAction, &QAction::triggered, this, &TextWidget::_q_replaceButtonClicked);
}

TextWidget::~TextWidget() {
}

QString TextWidget::sentence() const {
    QString words = wordsText->text();
    words.replace("\r\n", " ");
    words.replace("\n", " ");
    return words;
}

void TextWidget::_q_pasteButtonClicked() {
    auto board = QApplication::clipboard();
    QString text = board->text();
    if (!text.isEmpty()) {
        wordsText->setText(text);
    }
}

static QString filterString(const QString &str) {
    QString words;
    for (const auto &ch : str) {
        auto u = ch.unicode();
        if (u >= 128 || !ch.isLetter()) {
            if (words.isEmpty() || words.back() != ' ') {
                words.append(' ');
            }
            continue;
        }
        words.append(ch);
    }
    return words;
}

void TextWidget::_q_replaceButtonClicked() {
    QString str = languageCombo->currentIndex() == 0 ? g2p->convert(sentence()) : g2p_jp->kana2romaji(sentence());
    contentText->setPlainText(filterString(str));
}

void TextWidget::_q_appendButtonClicked() {
    QString str = languageCombo->currentIndex() == 0 ? g2p->convert(sentence()) : g2p_jp->kana2romaji(sentence());
    QString org = contentText->toPlainText();
    contentText->setPlainText((org.isEmpty() ? "" : org + " ") + filterString(str));
}
