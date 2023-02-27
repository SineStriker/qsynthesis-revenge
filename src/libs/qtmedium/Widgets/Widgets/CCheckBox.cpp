#include "CCheckBox.h"

CCheckBox::CCheckBox(QWidget *parent) : QAbstractButton(parent) {
    init();
}

CCheckBox::CCheckBox(const QString &text, QWidget *parent) : QAbstractButton(parent) {
    init();
    setText(text);
}

CCheckBox::~CCheckBox() {
}

void CCheckBox::init() {
    m_box = new CToolButton();
    m_box->setObjectName("box");
    m_box->setFocusPolicy(Qt::StrongFocus);
    m_box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_label = new QLabel();
    m_label->setObjectName("label");
    m_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    m_layout->addWidget(m_box);
    m_layout->addWidget(m_label);

    setLayout(m_layout);
    setCheckable(true);
    connect(m_box, &CToolButton::toggled, this, &CCheckBox::handleBoxToggled);
}

void CCheckBox::handleBoxToggled(bool checked) {
    setChecked(checked);
}

void CCheckBox::paintEvent(QPaintEvent *event) {
    bool checkable = this->isCheckable();
    bool checked = this->isChecked();
    QString text = this->text();
    if (checkable != m_box->isCheckable()) {
        m_box->setCheckable(checkable);
    }
    if (checked != m_box->isChecked()) {
        m_box->setChecked(checked);
    }
    if (text != m_label->text()) {
        m_label->setText(text);
    }
    return QWidget::paintEvent(event);
}

void CCheckBox::checkStateSet() {
    return QAbstractButton::checkStateSet();
}

void CCheckBox::nextCheckState() {
    return QAbstractButton::nextCheckState();
}
