#include "G2pWidget.h"
#include "cleanlyric.h"

G2pWidget::G2pWidget(QWidget *parent) : QWidget(parent) {
    m_textEditTop = new QTextEdit(this);
    m_textEditBottom = new QTextEdit(this);
    m_buttonConvert = new QPushButton("转换", this);
    m_buttonReorder = new QPushButton("填词", this);


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
    layout->addWidget(m_buttonReorder);
    layout->addWidget(m_textEditBottom);
    connect(m_buttonConvert, &QPushButton::clicked, this, &G2pWidget::convertLyric);
    connect(m_buttonReorder, &QPushButton::clicked, this, &G2pWidget::reorderLyric);
    setLayout(layout);
}

G2pWidget::~G2pWidget() {
}

void G2pWidget::convertLyric() {
    QString rawStr = m_textEditTop->toPlainText();
    QString res;
    auto cleanStr = m_checkBox->isChecked() ? IKg2p::CleanLyric::filterTextInParentheses(rawStr) : rawStr;
    if (m_radioBtnOption1->isChecked()) {
        qDebug() << "按字词隔开:";
        res = IKg2p::CleanLyric::splitLyricAsWord(cleanStr).join(" ");
    } else if (m_radioBtnOption2->isChecked()) {
        qDebug() << "按字符隔开:";
        res = IKg2p::CleanLyric::splitLyricAsChar(cleanStr).join(" ");
    }
    m_textEditBottom->setPlainText(res);
}

void G2pWidget::reorderLyric() {
    QString rawStr = m_textEditBottom->toPlainText();
    QString res = IKg2p::CleanLyric::reOrder(rawStr, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    m_textEditBottom->setPlainText(res);
}