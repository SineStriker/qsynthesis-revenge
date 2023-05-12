#include "MainWindow.h"

#include "cleanlyric.h"
#include <QApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    m_textEditTop = new QTextEdit(this);
    m_textEditBottom = new QTextEdit(this);
    m_buttonConvert = new QPushButton("转换", this);

    connect(m_buttonConvert, &QPushButton::clicked, this, &MainWindow::onConvertClicked);

    m_radioBtnOption1 = new QRadioButton("按字（单词）隔开", this);
    m_radioBtnOption2 = new QRadioButton("按字符隔开", this);
    m_radioBtnOption1->setChecked(true);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(m_radioBtnOption1);
    m_buttonGroup->addButton(m_radioBtnOption2);

    m_checkBox = new QCheckBox("删除括号中内容", this);
    m_checkBox->setGeometry(10, 10, 100, 30);

    auto *layout = new QVBoxLayout();
    layout->addWidget(m_textEditTop);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(m_radioBtnOption1);
    hLayout->addWidget(m_radioBtnOption2);
    hLayout->addWidget(m_checkBox);
    layout->addLayout(hLayout);

    layout->addWidget(m_buttonConvert);
    layout->addWidget(m_textEditBottom);

    auto *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);
    resize(800, 600);
}

MainWindow::~MainWindow() {
}

void MainWindow::onConvertClicked() {
    QString rawStr = m_textEditTop->toPlainText();

    auto cleanStr = m_checkBox->isChecked() ? IKg2p::CleanLyric::filterTextInParentheses(rawStr) : rawStr;
    if (m_radioBtnOption1->isChecked()) {
        qDebug() << "按字词隔开:";
        m_textEditBottom->setPlainText(IKg2p::CleanLyric::splitLyricAsWord(cleanStr).join(" "));
    } else if (m_radioBtnOption2->isChecked()) {
        qDebug() << "按字符隔开:";
        m_textEditBottom->setPlainText(IKg2p::CleanLyric::splitLyricAsChar(cleanStr).join(" "));
    }
}