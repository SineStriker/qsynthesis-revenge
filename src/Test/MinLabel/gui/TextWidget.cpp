#include "TextWidget.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

static const char PinyinHelperFileName[] = "pinyintool.exe";

TextWidget::TextWidget(QWidget *parent) : QWidget(parent) {
    finished = false;
    lastConvertAction = Replace;

    wordsText = new QLineEdit();
    wordsText->setPlaceholderText("Enter mandarin here...");
    contentText = new QPlainTextEdit();

    replaceButton = new QPushButton("Replace");
    appendButton = new QPushButton("Append");

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setMargin(0);
    buttonsLayout->addWidget(replaceButton);
    buttonsLayout->addWidget(appendButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(wordsText);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(contentText);

    setLayout(mainLayout);

    connect(replaceButton, &QPushButton::clicked, this, &TextWidget::_q_replaceButtonClicked);
    connect(appendButton, &QPushButton::clicked, this, &TextWidget::_q_appendButtonClicked);

    // Init pinyin helper
    pinyin = new QProcess(this);
    connect(pinyin, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &TextWidget::handleProcessFinished);
    connect(pinyin, &QProcess::errorOccurred, this, &TextWidget::handleReadError);
    connect(pinyin, &QProcess::readyReadStandardOutput, this, &TextWidget::handleReadOutput);
    connect(pinyin, &QProcess::readyReadStandardError, this, &TextWidget::handleReadError);

    pinyin->setProgram(PinyinHelperFileName);
    startTool();
}

TextWidget::~TextWidget() {
    finished = true;
    terminateTool();
}

void TextWidget::setText(const QString &text) {
    contentText->setPlainText(text);
}

QString TextWidget::text() const {
    return contentText->toPlainText();
}

void TextWidget::startTool() {
    pinyin->start();
    if (!pinyin->waitForStarted()) {
        QMessageBox::critical(this, qAppName(),
                              QString("Failed to load pinyin tool: %1!").arg(pinyin->program()));
        ::exit(-1);
    }
}

void TextWidget::terminateTool() {
    pinyin->kill();
    pinyin->waitForFinished();
}

void TextWidget::enterSentence() {
    QByteArray data = wordsText->text().toUtf8() + "\n";
    pinyin->write(data);
    pinyin->waitForBytesWritten();
}

void TextWidget::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus);
    if (!finished) {
        QMessageBox::warning(this, qAppName(),
                             QString("Pinyin tool exit with %1 unexpectedly! Try to restart.")
                                 .arg(QString::number(exitCode)));
        startTool();
    }
}

void TextWidget::handleProcessError(QProcess::ProcessError error) {
    Q_UNUSED(error);
}

void TextWidget::handleReadOutput() {
    auto bytes = pinyin->readAllStandardOutput();
    QString str = QString::fromUtf8(bytes).simplified();

    switch (lastConvertAction) {
        case Replace: {
            contentText->setPlainText(str);
            break;
        };
        case Append: {
            QString org = contentText->toPlainText();
            contentText->setPlainText(org.isEmpty() ? str : org + " " + str);
            break;
        }
    }
}

void TextWidget::handleReadError() {
}

void TextWidget::_q_replaceButtonClicked() {
    lastConvertAction = Replace;
    enterSentence();
}

void TextWidget::_q_appendButtonClicked() {
    lastConvertAction = Append;
    enterSentence();
}
