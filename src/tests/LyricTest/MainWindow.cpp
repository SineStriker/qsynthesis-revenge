#include "MainWindow.h"

#include <QApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    m_textEditTop = new QTextEdit(this);
    m_textEditBottom = new QTextEdit(this);
    m_buttonConvert = new QPushButton("转换", this);

    connect(m_buttonConvert, &QPushButton::clicked, this, &MainWindow::onConvertClicked);

    // 创建两个互斥的单选按钮
    m_radioBtnOption1 = new QRadioButton("按字（单词）隔开", this);
    m_radioBtnOption2 = new QRadioButton("按字符隔开", this);

    // 将两个单选按钮添加到同一个按钮组中
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(m_radioBtnOption1);
    m_buttonGroup->addButton(m_radioBtnOption2);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_textEditTop);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(m_radioBtnOption1);
    hLayout->addWidget(m_radioBtnOption2);
    layout->addLayout(hLayout);

    layout->addWidget(m_buttonConvert);
    layout->addWidget(m_textEditBottom);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);
    resize(800, 600);
}

MainWindow::~MainWindow() {
}

void MainWindow::onConvertClicked() {
    QString rawStr = m_textEditTop->toPlainText();

    if (m_radioBtnOption1->isChecked()) {
        qDebug() << "选项1被选中";
        m_textEditBottom->setPlainText(rawStr);
    } else if (m_radioBtnOption2->isChecked()) {
        qDebug() << "选项2被选中";
        m_textEditBottom->setPlainText(rawStr);
    }
}