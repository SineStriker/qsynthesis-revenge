//
// Created by fluty on 2023/8/6.
//

#include "LevelMeter.h"

class LevelMeterPrivate {
public:
    LevelMeter *q;

    QProgressBar *m_progressbar;
    QPushButton *m_button;
    QHBoxLayout *m_layout;

    explicit LevelMeterPrivate(LevelMeter *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);

        m_progressbar = new QProgressBar;
        m_progressbar->setObjectName("bar");
        m_progressbar->setFocusPolicy(Qt::StrongFocus);
        m_progressbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_progressbar->setTextVisible(false);
        m_progressbar->setMaximum(100);
        m_progressbar->setMinimum(0);

        m_button = new QPushButton;
        m_button->setObjectName("button");
        m_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

        m_layout = new QHBoxLayout;
        m_layout->setSpacing(0);
        m_layout->setMargin(0);
        m_layout->setObjectName("layout");
        m_layout->addWidget(m_progressbar);
        m_layout->addWidget(m_button);

        q->setLayout(m_layout);
    }
};

LevelMeter::LevelMeter(QWidget *parent) : QWidget(parent), d(new LevelMeterPrivate(this)) {
}

LevelMeter::~LevelMeter() {
    delete d;
}

void LevelMeter::paintEvent(QPaintEvent *event) {
    return QWidget::paintEvent(event);
}

void LevelMeter::setLevel(double level) {
    if (level > 1) {
        this->d->m_button->setText("clip");
        this->d->m_progressbar->setValue(100);
    } else {
        this->d->m_progressbar->setValue(level * 100);
    }
}
