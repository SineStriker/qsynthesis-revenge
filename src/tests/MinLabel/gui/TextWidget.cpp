#include "TextWidget.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>

TextWidget::TextWidget(QWidget *parent) : QWidget(parent), g2p(new IKg2p::ZhG2p()) {
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

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setMargin(0);
    buttonsLayout->addWidget(replaceButton);
    buttonsLayout->addWidget(appendButton);

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

void TextWidget::_q_replaceButtonClicked() {
    QString str = g2p->convert(sentence());
    contentText->setPlainText(str);
}

void TextWidget::_q_appendButtonClicked() {
    QString str = g2p->convert(sentence());
    QString org = contentText->toPlainText();
    contentText->setPlainText(org.isEmpty() ? str : org + " " + str);
}
